#include "rip.h"

void dfs(char *buf, int n, int index,  int balance, int l_rem, int r_rem)
{
	if (n == index)
	{
		if (balance == 0  && l_rem == 0 && r_rem == 0)
			put(buf);
		return ;
	}
	char c = buf[index];
	if (c == '(')
	{
		if (l_rem > 0)
		{
			buf[index] = ' ';
			dfs(buf, n, index + 1, balance, l_rem - 1, r_rem);
			buf[index] = '(';
		}
		dfs(buf, n, index + 1, balance + 1, l_rem, r_rem);
	}
	else if (c == ')')
	{
		if(r_rem > 0)
		{
			buf[index] = ' ';
			dfs(buf, n, index + 1, balance, l_rem, r_rem - 1);
			buf[index] = ')';
		}
		if (balance > 0)
			dfs(buf, n, index + 1, balance - 1, l_rem, r_rem);
	}

}

int main(int ac, char **av)
{
	if (ac != 2)
		return 1;
	int n = 0;
	int input = av[1];
	while(input[n])
		n++;
	char buf[n + 1];
	for (int i = 0; i <= n; i++)
		buf[i] = input[i];
	int l_rem = 0; r_rem = 0;
	for (int i = 0; i < n ; i++)
	{
		if (buf[i] == '(')
			l_rem++;
		else if (buf[i] == ')')
		{
			if (l_rem > 0)
				l_rem--;
			else 
				r_rem++;
		}
	}
	dfs(buf, n, 0, 0, l_rem, r_rem);
}
