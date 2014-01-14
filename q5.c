#include<stdio.h>
#include<assert.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<string.h>

void print_array(int **a, int num)
{
	int i, j;

	for (i = 0; i < num; i++) {
		for (j = 0; j < num; j++) {
			printf("%d\t", a[i][j]);
		}
		printf("\n");
	}
}

void format_array(int **a, int num, int type)
{
	int i, j;
	int *L, *R;
	int lindex = 0, rindex = 0;
	int x, k = 0;
	int edges;
	struct timespec time;

	for (i = 0; i < num; i++)
		a[i][i] = 0;

	clock_gettime(CLOCK_MONOTONIC, &time);
	srand(time.tv_nsec);
	switch(type) {
	case 0:	//A normal random adjacency matrix
		for (i = 0; i < num; i++) {
			for (j = i + 1; j < num; j++) {
				if (rand() % 100 < 50) {
					a[i][j] = 1;
					a[j][i] = 1;
				} else {
					a[i][j] = 0;
					a[j][i] = 0;
				}
			}
		}
		break;
	case 1: // A sparse graph with edge density 4 / |V|
		for (i = 0; i < num; i++)
			for (j = 0; j < num; j++)
				a[i][j] = 0;

		edges = 4;
		while (k < edges) {
			i = rand() % num;
			j = rand() % num;
			if (i != j) {
				a[i][j] = 1;
				a[j][i] = 1;
				k++;
			}
		}
		break;
	case 2:	// A bopartite graph with edge probability 50%
		// Format two bipartite node arrays
		L = malloc(num * sizeof(int) / 2);
		R = malloc(num * sizeof(int) / 2);

		for (i = 0; i < num; i++) {
			x = rand();
			if (lindex < num / 2 && x < 50)
				L[lindex++] = i;
			else if (rindex < num / 2 && x >= 50)
				R[rindex++] = i;
			else if (lindex == num / 2)
				R[rindex++] = i;
			else if (rindex == num / 2)
				L[lindex++] = i;
		}
		if (rindex != num / 2 || lindex != num / 2)
			assert(0);

		// Format edges from L to R
		for (i = 0; i < num / 2; i++) {
			for (j = 0; j < num / 2; j++) {
				if (rand() % 100 < 50) {
					a[L[i]][R[j]] = 1;
					a[R[j]][L[i]] = 1;
				} else {
					a[L[i]][R[j]] = 0;
					a[R[j]][L[i]] = 0;
				}
			}
		}

		free(L);
		free(R);
		break;
	default:
		break;
	}
//	printf("Matrix format finished\n");
}

int check_triangle(int **a, int num)
{
	int result = 0;
	int i, j, k;
	int *adj_list;
	int degree = 0;

	adj_list = malloc(num * sizeof(int));

	for (i = 0; i < num; i++) {
		degree = 0;
		//copy adjacent node numbers to adj_list
		for (j = 0; j < num; j++) {
			if (a[i][j])
				adj_list[degree++] = j;
		}

		for (j = 0; j < degree; j++) {
			for (k = j + 1; k < degree; k++) {
				if (a[adj_list[j]][adj_list[k]]) {
					result = 1;
					goto found;
				}
			}
		}
	}

found:
	if (result)
		printf("Triangle: %d %d %d\n", i, adj_list[j], adj_list[k]);

	free(adj_list);
	return result;
}

int main(int argc, char **argv)
{
	int n, i;
	int num_nodes;
	int **a;
	int result;
	int type;
	long long time;
	struct timespec start, end;
	FILE *output;
	char *filename;

	if (argc < 4) {
		printf("Format: ./q5 $num_nodes_in_log2 $grpah_type $output_file\n");
		return 0;
	}

	n = atoi(argv[1]);
	if (n <= 0) {
		printf("num_nodes_in_log2 must be larger than 0\n");
		return 0;
	}

	num_nodes = pow(2, n);
	a = malloc(num_nodes * sizeof(int *));
	for (i = 0; i < num_nodes; i++)
		a[i] = malloc(num_nodes * sizeof(int));

	type = atoi(argv[2]);
	if (type < 0 || type > 2) {
		printf("type must be 0(random graph with 50%% edge probability), 1(random sparse graph), or 2(bipartie grpah).\n");
		return 0;
	}

	filename = (char *)malloc(120 * sizeof(char));
	strcpy(filename, argv[3]);
	output = fopen(filename, "a");
	format_array(a, num_nodes, type);
//	print_array(a, num_nodes);

	clock_gettime(CLOCK_MONOTONIC, &start);
	result = check_triangle(a, num_nodes);
	clock_gettime(CLOCK_MONOTONIC, &end);

	time = (end.tv_sec - start.tv_sec) *1e9 + (end.tv_nsec - start.tv_nsec);

	if (result)
		printf("There is a triangle in the graph\n");
	else
		printf("There is not a triangle in the graph\n");

	printf("%d %d, Algorithm takes %lld microseconds\n", num_nodes, type, time / 1000);
	fprintf(output, "%d,%d,%lld\n", num_nodes, type, time / 1000);

	for (i = 0; i < num_nodes; i++)
		free(a[i]);
	free(a);
	free(filename);

	fclose(output);
	return 0;
}
