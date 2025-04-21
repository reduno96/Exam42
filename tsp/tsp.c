/* tsp.c */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXN 11

static float best_dist;

/*
 * dfs builds the route one city at a time:
 *  pos          = next position to fill (1..n-1)
 *  n            = total number of cities
 *  current_len  = length so far from city[0] through route[pos-1]
 *  route[]      = current ordering of cities (route[0] is fixed at 0)
 *  used[]       = flags for which cities are already in the route
 *  dist[i][j]   = precomputed distance between city i and city j
 */
static void
dfs(int pos, int n, float current_len, int route[], int used[], float dist[MAXN][MAXN])
{
    if (pos == n)
    {
        /* close the loop: last → first */
        current_len += dist[route[n - 1]][route[0]];
        if (current_len < best_dist)
            best_dist = current_len;
        return;
    }

    for (int city = 1; city < n; ++city)
    {
        if (!used[city])
        {
            float next_len = current_len + dist[route[pos - 1]][city];
            /* prune if already worse than best */
            if (next_len >= best_dist)
                continue;
            used[city] = 1;
            route[pos] = city;
            dfs(pos + 1, n, next_len, route, used, dist);
            used[city] = 0;
        }
    }
}

int main(void)
{
    float xs[MAXN], ys[MAXN];
    int n = 0;

    /* read all lines of the form "%f, %f\n" */
    while (n < MAXN && fscanf(stdin, "%f, %f\n", &xs[n], &ys[n]) == 2)
        ++n;

    if (n == 0)
    {
        /* no cities? just print 0.00 */
        fprintf(stdout, "0.00\n");
        return 0;
    }

    /* precompute pairwise distances */
    float dist[MAXN][MAXN];
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            float dx = xs[i] - xs[j];
            float dy = ys[i] - ys[j];
            dist[i][j] = sqrtf(dx * dx + dy * dy);
        }
    }

    /* set up for backtracking */
    int route[MAXN];
    int used[MAXN] = {0};
    route[0] = 0;
    used[0] = 1;
    best_dist = 1e30f; /* a large initial value */

    dfs(1, n, 0.0f, route, used, dist);

    /* print with two decimal places */
    fprintf(stdout, "%.2f\n", best_dist);
    return 0;
}
