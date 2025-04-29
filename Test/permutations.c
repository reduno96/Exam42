#include "permutations.h"

void backtraking(int depth, int n, char *orig, char *perm, char *used)
{
	if (depth == n)
	{
		perm[n] = '\0';
		puts(perm);
		return ;
	}
	for(int i = 0; i < n ; i++)
	{
		if (!used[i])
		{
			used[i] = 1;
			perm[depth] = orig[i];
			backtraking(depth + 1, n, orig, perm, used);
			used[i] = 0;
		}
	}
		
}
void generate_per(char *argv)
{
	char *str = argv;
	int n = 0;
	while(str[n])
		n++;
	for(int i = 0; i < n -1 ; i++)
	{
		for (int j = 0; j < n - i - 1; j++)
		{
			if (str[j] > str[j + 1])
			{
				char tmp = str[j];
				str[j] = str[j + 1];
				str[j + 1] = tmp;
			}
		}
	}
	char *perm = calloc(n + 1, sizeof(char));
	char *used = calloc(n, sizeof(char));
	if (!perm || !used)
	{
		free(perm);
		free(used);
		return ;
	}
	backtraking(0, n, str, perm, used);
	free(perm);
	free(used);
}
int main(int ac, char **av)
{
	if (ac != 2)
		return 0;
	generate_per(av[1]);
	return 0;
}
