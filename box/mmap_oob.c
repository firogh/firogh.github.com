#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{    
// dd if=/dev/urandom of=/tmp/mmapped.bin bs=512 count=1
    const char *filepath = "/tmp/mmapped.bin";

    int fd = open(filepath, O_RDWR, (mode_t)0600);
    
    if (fd == -1)
    {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }        
    
    struct stat fileInfo = {0};
    
    if (fstat(fd, &fileInfo) == -1)
    {
        perror("Error getting the file size");
        exit(EXIT_FAILURE);
    }
    
    if (fileInfo.st_size == 0)
    {
        fprintf(stderr, "Error: File is empty, nothing to do\n");
        exit(EXIT_FAILURE);
    }
    
    printf("File size is %ji\n", (intmax_t)fileInfo.st_size);
    
    char *map = mmap(0, 5, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED)
    {
        close(fd);
        perror("Error mmapping the file");
        exit(EXIT_FAILURE);
    }

    char *map2 = mmap(0, 1, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 4096);
    if (map2 == MAP_FAILED)
    {
        perror("Error mmapping the file");
    }


	// Outof boundary, since we just mapped 5 byte;
	char *p = map + 2046;
	*p = 'F';
	*(++p) = 'I';
	*(++p) = 'R';
	*(++p) = 'O';
	printf("fileInfo.st_size %d %p %p %p %d %d \n", *p, p, map,map2, map2 -map, p -map);
    
/*
    for (off_t i = 0; i < fileInfo.st_size; i++)
    {
        printf("Found character %c at %ji\n", map[i], (intmax_t)i);
    }
    
*/
    // Don't forget to free the mmapped memory
/*    if (munmap(map, fileInfo.st_size) == -1)
    {
        close(fd);
        perror("Error un-mmapping the file");
        exit(EXIT_FAILURE);
    }
*/

    // Un-mmaping doesn't close the file, so we still need to do that.
    close(fd);
    
    return 0;
}
