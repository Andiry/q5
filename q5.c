#include<stdio.h>
#include<stdlib.h>
#include<math.h>

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

int main(void)
{
	int n, i;
	int num_nodes;
	int **a;

	printf("Please input the number of nodes, in log2:\n");
	scanf("%d", &n);

	if (n <= 0)
		return 0;

	num_nodes = pow(2, n);
	a = malloc(num_nodes * sizeof(int *));
	for (i = 0; i < num_nodes; i++)
		a[i] = malloc(num_nodes + sizeof(int));

	print_array(a, num_nodes);

	return 0;
}
