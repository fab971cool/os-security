#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() 
{
    printf("ECOLE 2600 - My init program\n");
    while (2600)
    {
        // Infinite loop to keep the init process running
        pid_t pid = fork();
        int status = 0;
        if (pid)
        {
            waitpid(pid, &status, 0);
            printf("Infinite loop\n");
            pid = 0;
        }
        else
        {
            char *tab[] = {"usr/bin/setsid", "cttyhack", "sh", NULL};
            execv("/usr/bin/setsid", tab);
        }
    }
}