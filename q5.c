#include<stdio.h>
#include<assert.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

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

void format_array(int **a, int num, int prob, int bipar)
{
	int i, j;
	int *L, *R;
	int lindex = 0, rindex = 0;
	int x;

	for (i = 0; i < num; i++)
		a[i][i] = 0;

	if (bipar == 0) {
		//A normal random adjacency matrix
		srand(time(NULL));
		for (i = 0; i < num; i++) {
			for (j = i + 1; j < num; j++) {
				if (rand() % 100 < prob) {
					a[i][j] = 1;
					a[j][i] = 1;
				} else {
					a[i][j] = 0;
					a[j][i] = 0;
				}
			}
		}
	} else {
		// A bopartite graph with edge probability 50%
		// Format two bipartite node arrays
		L = malloc(num * sizeof(int) / 2);
		R = malloc(num * sizeof(int) / 2);

		srand(time(NULL));
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
	}
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
	int prob;
	int bipar;
	long long time;
	struct timespec start, end;


	if (argc < 4) {
		printf("Format: ./q5 $num_nodes_in_log2 $edge_density_in_percentage $if_bipartite\n");
		return 0;
	}

	n = atoi(argv[1]);
	if (n <= 0)
		return 0;

	num_nodes = pow(2, n);
	a = malloc(num_nodes * sizeof(int *));
	for (i = 0; i < num_nodes; i++)
		a[i] = malloc(num_nodes * sizeof(int));

	prob = atoi(argv[2]);
	bipar = atoi(argv[3]);
	format_array(a, num_nodes, prob, bipar);
//	print_array(a, num_nodes);

	clock_gettime(CLOCK_MONOTONIC, &start);
	result = check_triangle(a, num_nodes);
	clock_gettime(CLOCK_MONOTONIC, &end);

	time = (end.tv_sec - start.tv_sec) *1e9 + (end.tv_nsec - start.tv_nsec);

	if (result)
		printf("There is a triangle in the graph\n");
	else
		printf("There is not a triangle in the graph\n");

	printf("Algorithm takes %lld microseconds\n", time / 1000);

	for (i = 0; i < num_nodes; i++)
		free(a[i]);
	free(a);

	return 0;
}
