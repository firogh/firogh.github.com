#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */

int main(void)
{
        int fd, fd2;
        pid_t B;
        int fds[2], fds2[2];
        unsigned char *p, *p2;
        char *s = "/firo_shm", buf[56], *s2="Hello.\n";

        pipe(fds);
        pipe(fds2);
        B = fork();
        if (B) {
                printf("Parent.\n");
                memset(buf, 0, 56);
                read(fds[0], buf, 32);
                printf("parnet: %s\n", buf);
                write(1, buf, strlen(buf));
                printf("\nparent read shm path\n");

                fd2 = shm_open(buf, O_RDWR, S_IRUSR | S_IWUSR);
                p2 = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);
                memcpy(p2, s2, strlen(s2));
                write(fds2[1], "informs\n", strlen("informs\n"));

                return 0;
        }

        fd = shm_open(s, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

        printf("%d\n", fd);
        if (ftruncate(fd, 4096) == -1) {
                printf("ftruncate error.\n");
                goto out;
        }

        p = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (MAP_FAILED == p) {
                printf("mmap error.\n");
                goto out;
        }

	exit(0);

        memset(p , 0, 50);
        write(fds[1], s, strlen(s));
        printf("child write shm path %s\n", s);

        read(fds2[0], buf, 3);

        printf("child read shm memory %s\n", buf);


        write(1, p, 7);
        printf("child: end\n");

        return 0;
out:
        return 1;
}
