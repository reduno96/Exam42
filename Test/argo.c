#include "argo.h"

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

// this file is given in the exam to help you test your argo

void free_json(json j)
{
	switch (j.type)
	{
	case MAP:
		for (size_t i = 0; i < j.map.size; i++)
		{
			free(j.map.data[i].key);
			free_json(j.map.data[i].value);
		}
		free(j.map.data);
		break;
	case STRING:
		free(j.string);
		break;
	default:
		break;
	}
}

void serialize(json j)
{
	switch (j.type)
	{
	case INTEGER:
		printf("%d", j.integer);
		break;
	case STRING:
		putchar('"');
		for (int i = 0; j.string[i]; i++)
		{
			if (j.string[i] == '\\' || j.string[i] == '"')
				putchar('\\');
			putchar(j.string[i]);
		}
		putchar('"');
		break;
	case MAP:
		putchar('{');
		for (size_t i = 0; i < j.map.size; i++)
		{
			if (i != 0)
				putchar(',');
			serialize((json){.type = STRING, .string = j.map.data[i].key});
			putchar(':');
			serialize(j.map.data[i].value);
		}
		putchar('}');
		break;
	}
}
int parse_map(json *dst, FILE *stream)
{
	if (!expect(stream, '{'))
	{
		unexpected(stream);
		return -1;
	}
	dst->type = MAP;
	dst->map.data = NULL;
	dst->map.size = 0;
	if (!accept(stream, '}'))
		return 1;
	while(1)
	{
		dst->map.data = realloc(dst->map.data, dst->map.size + 1 );
		pair *cur = &dst->map.data[dst->map.size++];
		if (!(cur->key = get_str(stream)))
			return -1;
		if (!expect(stream, ':'))
			return -1;
		if (argo(&cur->value, stream) == -1)
			return -1;
		if (accept(stream, '}'))
			break;
		if (!expect(stream, ','))
			return -1;
	}
	return 1;
}
int parse_int(json *dst, FILE *stream)
{
	int sign = 1, c , res;

	if (accept(stream, '-'))
		sign = -1;
	if (!isdigit(c = getc(stream)))
	{
		unexpected(stream);
		return -1;
	}
	while(isdigit(c))
	{
		res = res * 10 + (c - '0');
		c = getc(stream);
	}
	if (!space(c) && c != '\n' && c != ',' && c != '}' && c != EOF)
		{
		unexpected(stream);
		return -1;
	}
	dst->type = INTEGER;
	dst->integer = res * sign;
	return 1;
}

char *get_str(FILE *stream)
{
	char *ret = calloc(4096, sizeof(char));
	int i = 0;
	int c  = getc ;

	while(1)
	{
		if (c == '"')
			break;
		if (c == EOF)
		{
			unexpected(stream);
			free(ret);
			return NULL;
		}
		if (c == '\\')
		{
			 c = getc(stream);
			if (c != '"' && c != '\\')
			{
				unexpected(stream);
				free(ret);
				return NULL;
			}
		}
		ret[i++] = c;
	}
	return ret;
}

int argo(json *dst, FILE *stream)
{
    int c = peek(stream);
    if (c == EOF)
    {
        unexpected(stream);
        return -1;
    }
    else if (isdigit(c), c == '-')
        return parse_int(dst, stream);
    else if (c = '"')
    {
        dst->type = STRING;
        if (!(dst->string = get_str(stream)))
            return -1;
        return 1;
    }
    else if (c = '{')
        return parse_json(dst, stream);
    unexpected(stream);
    return -1;
}

int main(int argc, char **argv)
{
	if (argc != 2)
		return 1;

	FILE *stream = fopen(argv[1], "r");
	if (!stream)
		return 1;

	json file = {0};

	if (argo(&file, stream) != 1)
	{
		fclose(stream);
		return 1; // clearly propagate failure
	}

	// check for leftover non-whitespace chars
	int trailing_char;
	do
	{
		trailing_char = getc(stream);
	} while (trailing_char != EOF && isspace(trailing_char));

	if (trailing_char != EOF)
	{
		ungetc(trailing_char, stream);
		unexpected(stream);
		free_json(file);
		fclose(stream);
		return 1;
	}

	serialize(file);
	printf("\n");

	free_json(file);
	fclose(stream);
	return 0;
}

