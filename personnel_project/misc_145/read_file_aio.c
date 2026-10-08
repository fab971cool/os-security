#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <aio.h>

int main(int argc, char *argv[])
{
    int fd = open(argv[1], O_RDONLY);

    if (fd == -1)
    {
        printf("open failed : %s\n", strerror(errno));
        return 1;
    }

    struct stat statbuf; // structure to hold file information

    if (stat(argv[1], &statbuf) == -1)
    {
        printf("stat failed : %s\n", strerror(errno));
        return 1;
    }

    char *content = malloc(statbuf.st_size + 1);
    struct aiocb cb;
    memset(&cb, 0, sizeof(struct aiocb));

    cb.aio_nbytes = statbuf.st_size;
    cb.aio_fildes = fd;
    cb.aio_offset = 0;
    cb.aio_buf = content;

    // aio_read permet de placer l'appel système dans une file coté noyau
    // et de continuer l'exécution du programme sans attendre la fin de l'appel système.
    if (aio_read(&cb) == -1)
    {
        printf("aio_read failed : %s\n", strerror(errno));
        return 1;
    }

    while (aio_error(&cb) == EINPROGRESS)
    {
        // wait for the read operation to complete
    }

    int read_bytes = aio_return(&cb);
    if (read_bytes == -1)
    {
        printf("aio_return failed : %s\n", strerror(errno));
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
