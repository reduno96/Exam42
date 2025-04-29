#include "n_queens.h"

void print_board(int *board, int n)
{
	for(int i  = 0; i < n - 1; i++ )
		printf("%i ", board[i]);
	printf("%i\n", board[n - 1]);

}
int is_safe(int *board, int n, int row, int col)
{
	for(int i = 0; i < row; i++)
		if (board[i] == col)
			return 0;
	for (int i = row, j = col; i >= 0 && j >= 0; i--, j--)
		if (board[i] == j)
			return 0;
	for (int i = row, j = col; i >= 0 && j < n; i--, j++)
		if (board[i] == j)
			return 0;
	return 1;
}
void solve(int *board, int n, int row)
{
	if (row == n)
	{
		print_board(board, n);
		return ;
	}

	for(int col = 0; col < n; col++)
	{
			if (is_safe(board, n, row, col))
			{
				board[row] = col;
				solve(board, n, row + 1);
				board[row] = -1;
			}
	}
}
int main(int ac, char **av)
{
	if (ac != 2)
		return 0;
	int n = atoi(av[1]);
	int *board = malloc(n  * sizeof(int));
	if (!board)
		return 1;
	for(int i = 0; i < n; i++)
		board[i] = -1;
	solve(board, n, 0);
	free(board);
}
