#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>


int main(int argc, char *argv[])
{
    int fd = open(argv[1], O_RDONLY);

    if (fd == -1) 
    {
        printf("open failed : %s\n", strerror(errno));
        return 1;
    }

    struct stat statbuf;
    
    if (stat(argv[1], &statbuf) == -1)
    {
        printf("stat failed : %s\n", strerror(errno));
        return 1;
    }

    char *content = malloc(statbuf.st_size +1);
    if (read(fd, content, statbuf.st_size) != statbuf.st_size)
    {
        printf("read failed : %s\n", strerror(errno));
        return 1;
    }

    content[statbuf.st_size] = '\0';
    printf("%s", content);

    free(content);
    if (close(fd) == -1)
    {
        printf("close failed : %s\n", strerror(errno));
        return 1;
    }

    return 0;
}
