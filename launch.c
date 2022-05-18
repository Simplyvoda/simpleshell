#include "main.h"

/**
 * sh_launch - launches program and waits for it to terminate
 *
 * @args: Null terminated list of arguments
 *
 * Return: integer(1)
 */

int sh_launch(char **args)
{
	pid_t pid;
	int status;

	pid = fork();

	if (pid == 0)
	{
		if (execvp(args[0], args) == -1)
		{
			perror("lsh");
			exit(EXIT_FAILURE);
		}
		else if (pid < 0)
		{
			perror("lsh");
		}
		else
		{
			do {
				waitpid(pid, &status, WUNTRACED);
			} while (!WIFEXITED(status) && !WIFSIGNALED(status));
		}

		return (1);
	}

	return (1);
}
