#include "argo.h"
#include <string.h>
#include <ctype.h>

int peek(FILE *stream)
{
	int c = getc(stream);
	ungetc(c, stream);
	return c;
}

void unexpected(FILE *stream)
{
	if (peek(stream) != EOF)
		printf("Unexpected token '%c'\n", peek(stream));
	else
		printf("Unexpected end of input\n");
}

int accept(FILE *stream, char c)
{
	if (peek(stream) == c)
	{
		getc(stream);
		return 1;
	}
	return 0;
}

int expect(FILE *stream, char c)
{
	if (accept(stream, c))
		return 1;
	unexpected(stream);
	return 0;
}

int parse_int(json *dst, FILE *stream)
{
	int c, sign = 1, n = 0;

	if (accept(stream, '-'))
		sign = -1;

	if (!isdigit(peek(stream)))
	{
		unexpected(stream);
		return -1;
	}

	while (isdigit((c = peek(stream))))
	{
		getc(stream);
		n = n * 10 + (c - '0');
	}

	if (!isspace(c) && c != EOF && c != ',' && c != '}' && c != '\n')
	{
		unexpected(stream);
		return -1;
	}

	dst->type = INTEGER;
	dst->integer = n * sign;
	return 1;
}

char *get_str(FILE *stream)
{
	char *res = calloc(4096, sizeof(char));
	int i = 0;
	char c = getc(stream);

	while (1)
	{
		c = getc(stream);

		if (c == '"')
			break;
		if (c == EOF)
		{
			unexpected(stream);
			free(res);
			return NULL;
		}
		if (c == '\\')
		{
			c = getc(stream);
			if (c != '"' && c != '\\')
			{
				printf("Unexpected token '%c'\n", c); // Directly print the invalid character
				free(res);
				return NULL;
			}
		}
		res[i++] = c;
	}
	return (res);
}

int parse_map(json *dst, FILE *stream)
{
	if (!expect(stream, '{'))
		return -1;
	dst->type = MAP;
	dst->map.size = 0;
	dst->map.data = NULL;

	if (accept(stream, '}'))
		return 1;

	while (1)
	{

		if (peek(stream) != '"')
		{
			unexpected(stream);
			return -1;
		}
		dst->map.data = realloc(dst->map.data, sizeof(pair) * (dst->map.size + 1));
		pair *current = &dst->map.data[dst->map.size++];
		current->key = get_str(stream);
		if (!current->key)
			return -1;
		if (!expect(stream, ':'))
			return -1;
		if (argo(&current->value, stream) == -1)
			return -1;

		if (accept(stream, '}'))
			break;
		if (!expect(stream, ','))
			return -1;
	}
	return 1;
}

int argo(json *dst, FILE *stream)
{
	int c = peek(stream);

	if (c == EOF)
	{
		unexpected(stream);
		return -1;
	}
	else if (isdigit(c) || c == '-')
		return parse_int(dst, stream);
	else if (c == '"')
	{
		dst->type = STRING;
		if (!(dst->string = get_str(stream)))
			return -1;
		return 1;
	}
	else if (c == '{')
		return parse_map(dst, stream);

	unexpected(stream);
	return -1; // Just return -1 immediately after reporting the error
}
