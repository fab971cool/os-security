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
        if (pid < 0)
        {
            perror("fork failed");
            sleep(1);
            continue;
        }
        if (pid)
        {
            int status = 0;
            waitpid(pid, &status, 0);
            printf("Infinite loop\n");
            pid = 0;
        }
        else
        {
            // définition des arguments de execv
            char *tab[] = {"setsid", "cttyhack", "sh", NULL};
            execv("/usr/bin/setsid", tab);
            perror("execv failed");
            _exit(127);
        }
    }
}