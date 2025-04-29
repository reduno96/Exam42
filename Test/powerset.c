// #include "powerset.h"
#include <stdio.h>
#include <stdlib.h>

void print_subset(int *subset, int len_sub)
{
    for (int i = 0; i < len_sub; i++)
    {
        fprintf(stdout, "%d", subset[i]);
        if (i < len_sub - 1)
            fprintf(stdout, " ");
    }
    fprintf(stdout, "\n");
}


void backtraking(int *arr, int n, int target, int *subset, int len_sub, int sum, int pos)
{
	if (sum == target && len_sub > 0){
		print_subset(subset, len_sub);
		return ;
	}
	if (pos == n)
		return ;
	subset[len_sub] = arr[pos];
	backtraking(arr, n, target, subset, len_sub + 1, sum + arr[pos], pos + 1);
	backtraking(arr, n, target, subset, len_sub, sum, pos + 1);

}
int main(int ac, char **av)
{
	if (ac < 2)
		return 0;
	
	int target = atoi(av[1]);
	
	int n = ac - 2;
	int *arr = malloc(n * sizeof(int));
	if (!arr)
		return 1;
	for (int i = 0; i < n; i++)
		arr[i] = atoi(av[i + 2]);
	int *subset = malloc(n * sizeof(int ));
	if (!subset)
		return (free(arr), 1);
	backtraking(arr, n, target, subset, 0, 0, 0);
	free(arr);
    free(subset);
}
