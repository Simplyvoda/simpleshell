#include <stdio.h>
#include <unistd.h>

/**
 * main - returns process ID
 *
 * Return: integer
 */

int main()
{
    int p_id, pp_id;

    p_id = getpid(); /*process id*/
    pp_id = getppid(); /*parent process id*/

    printf("Process ID: %d\n", p_id);
    printf("Parent Process ID: %d\n", pp_id);

    return 0;
}
