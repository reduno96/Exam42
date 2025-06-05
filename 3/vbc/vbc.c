#include "vbc.h"

node *parse_highest(char **s)
{
    if (accept(s, '('))
    {
        node *n = parse_lowest(s);
        if (!n)
            return NULL;
        if (!expect(s, ')'))
        {
            destroy_tree(n);
            return NULL;
        }
        return n;
    }
    else if (isdigit(**s))
    {
        int value = **s - '0';
        (*s)++;
        node temp = {.type = VAL, .val = value, .l = NULL, .r = NULL};
        return new_node(temp);
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
        node temp = {.type = MULTI, .l = left, .r = right};
        left = new_node(temp);
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
        node temp = {.type = ADD, .l = left, .r = right};
        left = new_node(temp);
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
    return (ret);
}
