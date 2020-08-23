#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int fd, type;

	if(3 != argc)
	{
		printf("\n Usage :  type, file_name\n");
		return 1;
	}

	errno = 0;
	type = atoi(argv[1]);
	switch (type)
	{
		case 1:
			fd = open(argv[2],O_TRUNC|O_CREAT,S_IRWXU);

			if(-1 == fd)
			{
				printf("\n open() failed with error [%s]\n",strerror(errno));
				return 1;
			}
			break;
		case 2:
			fd = open(argv[2],O_TRUNC,S_IRWXU);

			if(-1 == fd)
			{
				printf("\n open() failed with error [%s]\n",strerror(errno));
				return 1;
			}
			break;
		case 3:
			fd = open(argv[2],O_EXCL|O_CREAT,S_IRWXU);

			if(-1 == fd)
			{
				printf("\n open() failed with error [%s]\n",strerror(errno));
				return 1;
			}
			break;
		default:
			printf("%s \n", argv[1]);
			return 0;
	}
	printf("\n Open() Successful.\n");

    return 0;
}
