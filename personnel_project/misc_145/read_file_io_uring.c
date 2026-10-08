/*
io_uring utilise le ring buffer entre l'espace uttilisateur et le noyau.
Cela évite les copies de données entre l'espace utilisateur et le noyau.

*/

#include <errno.h>
#include <fcntl.h>
#include <liburing.h> // bibliothèque pour utiliser io_uring
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

    // mise en place du ring buffer avec 1 espace dans la file
    struct io_uring ring;
    int status = io_uring_queue_init(1, &ring, 0);

    if (status != 0)
    {
        printf("io_uring_queue_init failed");
        return 1;
    }

    // preparation de l'appel système "open" asynchrone de io_uring
    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
    io_uring_prep_open(sqe, argv[1], O_RDONLY, 0);
    io_uring_submit(&ring);

    // permet d'insérer l'appel système dans le ring buffer
    struct io_uring_cqe *cqe;
    int ret = io_uring_wait_cqe(&ring, &cqe);
    io_uring_cqe_seen(&ring, cqe);

    if (ret < 0)
    {
        perror("io_uring_wait_cqe");
        return 1;
    }

    if (cqe->res < 0)
    {
        fprintf(stderr, "Async operation failed: %s\n", strerror(cqe->res));
        errno = -cqe->res;
        return -1;
    }

    int fd = cqe->res;
    struct stat statbuf;

    if (stat(argv[1], &statbuf) == -1)
    {
        printf("stat failed : %s", strerror(errno));
        return 1;
    }

    char *content = malloc(statbuf.st_size + 1);

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
}