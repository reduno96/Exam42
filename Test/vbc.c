#include "vbc.h"

node *new_node(node n)
{
	node *ret = calloc(1, sizeof(n));
	if (!ret)
		return (NULL);
	*ret = n;
	return (ret);
}

void destroy_tree(node *n)
{
	if (!n)
		return;
	if (n->type != VAL)
	{
		destroy_tree(n->l);
		destroy_tree(n->r);
	}
	free(n);
}

void unexpected(char c)
{
	if (c)
		fprintf(stderr, "Unexpected token '%c'\n", c);
	else
		fprintf(stderr, "Unexpected end of input\n");
}

int accept(char **s, char c)
{
	if (**s == c)
	{
		(*s)++;
		return (1);
	}
	return (0);
}

int expect(char **s, char c)
{
	if (accept(s, c))
		return (1);
	unexpected(**s);
	return (0);
}

int eval_tree(node *tree)
{
	switch (tree->type)
	{
	case ADD:
		return (eval_tree(tree->l) + eval_tree(tree->r));
	case MULTI:
		return (eval_tree(tree->l) * eval_tree(tree->r));
	case VAL:
		return (tree->val);
	}
	return 0;
}
int main(int argc, char **argv)
{
	if (argc != 2)
		return (1);
	node *tree = parse_expr(argv[1]);
	if (!tree)
		return (1);
	printf("%d\n", eval_tree(tree));
	destroy_tree(tree);
	return 0;
}
node *parse_highest(char **s)
{
	if (accept(s, '('))
	{
		node *left = parse_lowest(s);
		if (!expect(s, ')'))
		{
			destroy_tree(left);
			return NULL;
		}
		return left;
	}
	else if (isdigit(**s))
	{
		int value = **s - '0';
		(*s)++;
		node tmp = {.type = VAL, .val = value, .l = NULL, .r = NULL};
		node *left = new_node(tmp);
		return left;
	}
	else
	{
		unexpected(**s);
		return NULL;
	}
}
node *parse_middle(char **s)
{
	node *left = parse_highest(s);
	if (!left)
		return NULL;
	while (accept(s, '*'))
	{
		node *right = parse_highest(s);
		if (!right)
		{
			destroy_tree(left);
			return NULL;
		}
		node tmp = {.type = MULTI, .l = left, .r = right};
		left = new_node(tmp);
		if (!left)
			return NULL;
	}
	return left;
}
node *parse_lowest(char **s)
{
	node *left = parse_middle(s);
	if (!left)
		return NULL;
	while (accept(s, '+'))
	{
		node *right = parse_middle(s);
		if (!right)
		{
			destroy_tree(left);
			return NULL;
		}
		node tmp = {.type = ADD, .l = left, .r = right};
		left = new_node(tmp);
		if (!left)
			return NULL;
	}
	return left;
}
node *parse_expr(char *s)
{
	node *ret = parse_lowest(&s);
	if (*s && ret != NULL)
	{
		unexpected(*s);
		destroy_tree(ret);
		return NULL;
	}
	return ret;
}
