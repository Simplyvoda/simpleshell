#include "main.h"

/**
 * num_builtins - function to return number of builtins
 *
 * Return: void
 */
int num_builtins(void)
{
	char *builtin_str[] = {
		"cd",
		"exit"};

	int nb;

	nb = (sizeof(builtin_str) / sizeof(char *));

	return (nb);
}

/**
 * builtin_cd - function that changes directory
 *
 * @args: pointer to arguments
 *
 * Return: integer(1)
 */

int builtin_cd(char **args)
{
	if (args[1] == NULL)
	{
		fprintf(stderr, "No such file or directory\n");
	}
	else
	{
		if (chdir(args[1]) != 0)
		{
			perror("No such file or directory\n");
		}
	}
	return (1);
}

/**
 * builtin_exit - exit function
 *
 * @args: list of arguments
 *
 * Return: integer(0)
 */

int builtin_exit(char **args)
{
	if (strcmp("exit", args[0]) == 0)
	{
		return (0);
	}

	return (0);
}
