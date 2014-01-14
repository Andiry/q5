#include<stdio.h>
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

void format_array(int **a, int num, int prob)
{
	int i, j;

	srand(time(NULL));
	for (i = 0; i < num; i++) {
		for (j = i; j < num; j++) {
			if (i != j && rand() % 100 < prob) {
				a[i][j] = 1;
				a[j][i] = 1;
			} else {
				a[i][j] = 0;
				a[j][i] = 0;
			}
		}
	}
}

int main(void)
{
	int n, i;
	int num_nodes;
	int **a;
	int result;
	int prob;

	printf("Please input the number of nodes, in log2:\n");
	scanf("%d", &n);

	if (n <= 0)
		return 0;

	num_nodes = pow(2, n);
	a = malloc(num_nodes * sizeof(int *));
	for (i = 0; i < num_nodes; i++)
		a[i] = malloc(num_nodes + sizeof(int));

	printf("Please input the probability of edge, in percentage:\n");
	scanf("%d", &prob);
	format_array(a, num_nodes, prob);
	print_array(a, num_nodes);
//	result = check_triangle(a, num_nodes);

	if (result)
		printf("There is a triangle in the graph\n");
	else
		printf("There is not a triangle in the graph\n");

	return 0;
}
