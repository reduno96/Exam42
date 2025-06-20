#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct node
{
	enum
	{
		ADD,
		MULTI,
		VAL
	} type;
	int val;
	struct node *l;
	struct node *r;
} node;

node *new_node(node n);
void unexpected(char c);
int accept(char **s, char c);
int expect(char **s, char c);
void destroy_tree(node *n);
int eval_tree(node *tree);
node *parse_expr(char *s);
node *parse_lowest(char **s);
node *parse_middle(char **s);
node *parse_highest(char **s);
