#include <stdio.h>

void dfs(char *buf, int n, int index, int balance, int left, int right)
{
    if (index == n)
    {
        if (balance == 0 && left == 0 && right == 0)
        {
            buf[n] = 0;
            puts(buf);
        }
        return;
    }
    char c = buf[index];
    if (c == '(')
    {
        if (left > 0)
        {
            buf[index] = ' ';
            dfs(buf, n, index + 1, balance, left - 1, right);
            buf[index] = '(';
        }       
        dfs(buf, n, index + 1, balance + 1, left, right);
    }
    else    if (c == ')')
    {
        if (right > 0)
        {
            buf[index] = ' ';
            dfs(buf, n, index + 1, balance, left, right - 1);
            buf[index] = ')';
        }
        if (balance > 0)      
        dfs(buf, n, index + 1, balance - 1, left, right);
    }
    else if (c == ')')
    {   
        if (right > 0)
        {
            buf[index] = ' ';
            dfs(buf, n, index + 1, balance, left, right - 1);
            buf[index] = ')';
        }  
        if (balance > 0)
        {
            dfs(buf, n, index + 1, balance - 1, left, right);
        }
    }
    else
    {  
        dfs(buf, n, index + 1, balance, left, right);
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return 1;

    char *input = argv[1];
    int n = 0;
    while (input[n] != '\0')
        n++;
    if (n == 0)
    {
        return 0; 
    }
    char buf[n + 1];
    for (int i = 0; i <= n; i++)
        buf[i] = input[i];
    int left = 0, right = 0;
    for (int i = 0; i < n; i++)
    {
        if (buf[i] == '(')
        {
            left++;
        }
        else if (buf[i] == ')')
        {
            if (left > 0)
            {
                left--;
            }
            else
            {
                right++;
            }
        }
    }

    
    dfs(buf, n, 0, 0, left, right);
    return 0;
}
