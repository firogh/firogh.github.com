#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define _GNU_SOURCE
#define __USE_GNU
#include <dlfcn.h>


int kill (pid_t pid, int signum) __attribute__((weak, alias("__vdso_func")));
int __vdso_func(pid_t pid, int signum)
{

	printf("Firo %s %d %d\n", __func__, __LINE__, signum);

	return 0;
}

int main(int argc, char **argv)
{
	struct timeval start, end;
	pid_t pid;
	char buf[256];
	int i = kill(1,1);
	gettimeofday(&start, NULL);
	gettimeofday(&end, NULL);
	pid = getpid();
	sprintf(buf, "cat /proc/%d/maps", (int)pid);
	system(buf);	

	sprintf(buf, "cat /proc/%d/cmdline", (int)pid);
	system(buf);	
	sprintf(buf, "ldd $(cat /proc/%d/cmdline)", (int)pid);
	system(buf);	
	printf("\n%d %0x %0x %p %p\n", pid, &gettimeofday, &getpid, dlsym(RTLD_NEXT, "gettimeofday"), dlsym(RTLD_NEXT, "getpid"));
	printf("\n%d %p %p\n", pid, gettimeofday, dlsym(RTLD_NEXT, "__vdso_gettimeofday"));
	printf("%ld\n", ((end.tv_sec * 1000000 + end.tv_usec)
		  - (start.tv_sec * 1000000 + start.tv_usec)));

	return 0;
}
