/* n_queens.h */
#ifndef N_QUEENS_H
#define N_QUEENS_H

#include <stdio.h>

/* is it safe to place a queen at (row, col)? */
int  safe(int *pos, int col, int row);

/* recursively place queens starting at column ‘col’ */
void solve(int *pos, int n, int col);

#endif /* N_QUEENS_H */

