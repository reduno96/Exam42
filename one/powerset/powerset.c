#include "powerset.h"

void print_subset(int *current, int current_len)
{
    for (int i = 0; i < current_len; i++)
    {
        fprintf(stdout, "%d", current[i]);
        if (i < current_len - 1)
            fprintf(stdout, " ");
    }
    fprintf(stdout, "\n");
}

void backtrack(int *s, int m, int target, int *current, int current_len, int running_sum, int pos)
{
    if (pos == m)
    {
        if (running_sum == target && current_len > 0)
            print_subset(current, current_len);
        return;
    }
    // Exclude current element
    backtrack(s, m, target, current, current_len, running_sum, pos + 1);
    // Include current element
    current[current_len] = s[pos];
    backtrack(s, m, target, current, current_len + 1, running_sum + s[pos], pos + 1);
}

int main(int argc, char **argv)
{
    if (argc < 2)
        return 0;
    int target = atoi(argv[1]);
    int m = argc - 2;
    int *s = malloc(m * sizeof(int));
    if (!s)
        return 1;
    for (int i = 0; i < m; i++)
        s[i] = atoi(argv[i + 2]);
    int *current = malloc(m * sizeof(int));
    if (!current)
    {
        free(s);
        return 1;
    }
    backtrack(s, m, target, current, 0, 0, 0);
    free(s);
    free(current);
    return 0;
}
