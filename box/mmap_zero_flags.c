#include<stdio.h>
#include<sys/mman.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<string.h>
#define SIZE  1024*1024*1024
int main(int argc,char* argv[]) {
    int fd;
    struct stat sb;  
    char *p;
        if ((fd = open(argv[1], O_RDWR)) < 0) {
        perror("open");
        }  
    if ((fstat(fd, &sb)) == -1) {  
        perror("fstat");  
    }
    if ((p = (char *)mmap(NULL,sb.st_size, PROT_READ |   
                PROT_WRITE, 0, fd, 0)) == (void *)-1) {  
        perror("mmap");  
         }
        memset(p,'c',sb.st_size);
        sleep(100);
        return 0;
}
