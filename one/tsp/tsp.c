
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXN 11

static float best_dist;

static void dfs(int pos, int n, float current_len, int route[], int used[], float dist[MAXN][MAXN])
{
    if (pos == n)
    {
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
    while (n < MAXN && fscanf(stdin, "%f, %f\n", &xs[n], &ys[n]) == 2)
        ++n;
    if (n == 0)
    {
        fprintf(stdout, "0.00\n");
        return 0;
    }
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
    int route[MAXN];
    int used[MAXN] = {0};
    route[0] = 0;
    used[0] = 1;
    best_dist = 1e30f;
    dfs(1, n, 0.0f, route, used, dist);
    fprintf(stdout, "%.2f\n", best_dist);
    return 0;
}
