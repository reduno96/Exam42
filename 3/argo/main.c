/*
#include "argo.h"

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

int main(int argc, char **argv)
{
	if (argc != 2)
		return 1;
	char *filename = argv[1];
	FILE *stream = fopen(filename, "r");
	json file;
	if (argo(&file, stream) != 1)
	{
		free_json(file);
		return 1;
	}
	serialize(file);
	printf("\n");
	free_json(file);
}
*/
 #include "argo.h"

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
 
