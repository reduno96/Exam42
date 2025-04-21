#ifndef POWERSET_H
#define POWERSET_H
#include <stdio.h>
#include <stdlib.h>
#include "powerset.h"
void print_subset(int *current, int current_len);
void backtrack(int *s, int m, int target, int *current, int current_len, int running_sum, int pos);

#endif // POWERSET_H
