#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
  Function Declarations for builtin shell commands:
  these functions should be added as prototypes on main.h
  should be removed later
 */
int builtin_cd(char **args);
int builtin_help(char **args);
int builtin_exit(char **args);

/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
    "cd",
    "help",
    "exit"};

int (*builtin_func[])(char **) = {
    &builtin_cd,
    &builtin_help,
    &builtin_exit};

int sh_num_builtins()
{
    return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/

/**
   builtin_cd - change directory.

   @args: List of args.  args[0] is "cd".  args[1] is the directory.

   Return: Always returns 1, to continue executing.
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
            perror("lsh");
        }
    }
    return 1;
}

/**
   builtin_help - print help.
   @args:  List of args
   Return: Always returns 1, to continue executing.
 */
int builtin_help(char **args)
{
    int i;
    printf("An ALX project shell\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");

    for (i = 0; i < sh_num_builtins(); i++)
    {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

/**
  builtin_exit: shell exit function.
  @args List of args.  Not examined.
  Return: Always returns 0, to terminate execution.
 */
int builtin_exit(char **args)
{
    return 0;
}

/**
  sh_launch - Launch a program and wait for it to terminate.
  @args:  Null terminated list of arguments (including program).
  Return: Always returns 1, to continue execution.
 */
int sh_launch(char **args)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        // Child process
        if (execvp(args[0], args) == -1)
        {
            perror("lsh");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        // Error forking
        perror("lsh");
    }
    else
    {
        // Parent process
        do
        {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

/**
   sh_execute - Execute shell built-in to launch program.
   @args: Null terminated list of arguments.
   Return: 1 if the shell should continue running, 0 if it should terminate
 */
int sh_execute(char **args)
{
    int i;

    if (args[0] == NULL)
    {
        // An empty command was entered.
        return 1;
    }

    for (i = 0; i < sh_num_builtins(); i++)
    {
        if (strcmp(args[0], builtin_str[i]) == 0)
        {
            return (*builtin_func[i])(args);
        }
    }

    return sh_launch(args);
}

/**
   sh_read_line - Read a line of input from stdin.
   Return: The line from stdin.
 */
char *sh_read_line(void)
{
#define LSH_USE_STD_GETLINE // maybe create a newfile for getline function
    char *line = NULL;
    ssize_t bufsize = 0; // have getline allocate a buffer for us
    if (getline(&line, &bufsize, stdin) == -1)
    {
        if (feof(stdin))
        {
            exit(EXIT_SUCCESS); // We received an EOF
        }
        else
        {
            perror("lsh: getline\n");
            exit(EXIT_FAILURE);
        }
    }
    return line;
}


#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
/**
   @brief Split a line into tokens (very naively).
   @param line The line.
   @return Null-terminated array of tokens.
 */
char **sh_split_line(char *line)
{
    int bufsize = LSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
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
    return tokens;
}

/**
   sh_loop - Loop getting input and executing it.
 */
void sh_loop(void)
{
    char *line;
    char **args;
    int status;

    do
    {
        printf("$ ");
        line = sh_read_line();
        args = sh_split_line(line);
        status = sh_execute(args);

        free(line);
        free(args);
    } while (status);
}

/**
   main - Main entry point.
   @argc: Argument count.
   @argv: Argument vector.
   Return: status code
 */
int main(int argc, char **argv)
{
    // Load config files, if any.

    // Run command loop.
    sh_loop();

    // Perform any shutdown/cleanup.

    return EXIT_SUCCESS;
}
