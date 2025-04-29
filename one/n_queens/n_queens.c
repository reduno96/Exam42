
#include "n_queens.h"
#include <stdio.h>
#include <stdlib.h>

int safe(int *pos, int col, int row)
{
	int r;
	int diff;

	for (int c = 0; c < col; c++)
	{
		r = pos[c];
		diff = col - c;
		if (r == row || r + diff == row || r - diff == row)
			return (0);
	}
	return (1);
}

void solve(int *pos, int n, int col)
{
	if (col == n)
	{
		for (int i = 0; i < n; i++)
		{
			if (i)
				fprintf(stdout, " ");
			fprintf(stdout, "%d", pos[i]);
		}
		fprintf(stdout, "\n");
		return;
	}
	for (int row = 0; row < n; row++)
	{
		if (safe(pos, col, row))
		{
			pos[col] = row;
			solve(pos, n, col + 1);
		}
	}
}

int main(int argc, char **argv)
{
	int n;
	int *pos;

	if (argc != 2)
		return (1);
	n = atoi(argv[1]);
	if (n < 1)
		return (0);
	pos = calloc(n, sizeof *pos);
	if (!pos)
		return (1);
	solve(pos, n, 0);
	free(pos);
	return (0);
}
