#include "main.h"

/**
 * sh_read_line - function to readline from stdin
 *
 * Return: line from stdin
 */

char *sh_read_line(void)
{
#define LSH_USE_STD_GETLINE
	char *line = NULL;
	size_t bufsize = 0;

	if (getline(&line, &bufsize, stdin) == -1)
	{
		if (feof(stdin))
		{
			exit(EXIT_SUCCESS);
		}
		else
		{
			perror("lsh: geline\n");

			exit(EXIT_FAILURE);
		}
	}
	return (line);
}

/**
 * sh_split_line - splits lines
 *
 * @line: stdin from user
 *
 * Return: Null-terminated array of tokens.
 */

char **sh_split_line(char *line)
{
	int bufsize = LSH_TOK_BUFSIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char));
	char *token, **tokens_backup;

	if (!tokens)
	{
		fprintf(stderr, "lsh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, LSH_TOK_DELIM);
	while (token != NULL)
	{
		tokens[position] = token;
		position++;

		if (position >= bufsize)
		{
			bufsize += LSH_TOK_BUFSIZE;
			tokens_backup = tokens;
			tokens = realloc(tokens, bufsize * sizeof(char *));

			if (!tokens)
			{
				free(tokens_backup);
				fprintf(stderr, "lsh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, LSH_TOK_DELIM);
	}
	tokens[position] = NULL;
	return (tokens);
}
