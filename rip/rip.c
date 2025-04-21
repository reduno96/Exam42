#include <stdio.h>

void dfs(char *buf, int n, int index, int balance, int l_rem, int r_rem)
{
    if (index == n)
    {
        if (balance == 0 && l_rem == 0 && r_rem == 0)
        {
            puts(buf);
        }
        return;
    }
    char c = buf[index];
    if (c == '(')
    {
        // Option 1: remove this '('
        if (l_rem > 0)
        {
            buf[index] = ' ';
            dfs(buf, n, index + 1, balance, l_rem - 1, r_rem);
            buf[index] = '(';
        }
        // Option 2: keep it
        dfs(buf, n, index + 1, balance + 1, l_rem, r_rem);
    }
    else if (c == ')')
    {
        // Option 1: remove this ')'
        if (r_rem > 0)
        {
            buf[index] = ' ';
            dfs(buf, n, index + 1, balance, l_rem, r_rem - 1);
            buf[index] = ')';
        }
        // Option 2: keep it (only if there is a matching '(')
        if (balance > 0)
        {
            dfs(buf, n, index + 1, balance - 1, l_rem, r_rem);
        }
    }
    else
    {
        // Already removed (space)
        dfs(buf, n, index + 1, balance, l_rem, r_rem);
    }
}

// ...existing code...

int main(int argc, char **argv)
{
    if (argc != 2)
        return 1;

    char *input = argv[1];
    int n = 0;
    while (input[n] != '\0')
        n++;

    // Handle empty input case explicitly
    if (n == 0)
    {
        return 0; // Nothing to print
    }

    // Copy into a modifiable buffer
    char buf[n + 1];
    for (int i = 0; i <= n; i++)
        buf[i] = input[i];

    // Compute how many '(' and ')' we must remove
    int l_rem = 0, r_rem = 0;
    for (int i = 0; i < n; i++)
    {
        if (buf[i] == '(')
        {
            l_rem++;
        }
        else if (buf[i] == ')')
        {
            if (l_rem > 0)
            {
                l_rem--;
            }
            else
            {
                r_rem++;
            }
        }
    }

    // Recurse and print all minimal-removal solutions
    dfs(buf, n, 0, 0, l_rem, r_rem);
    return 0;
}
