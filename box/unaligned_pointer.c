#include <stdio.h>
#include <stdlib.h>


int main(void)
{
	void *p = malloc(1024);
	unsigned char *c = NULL;
	printf("addr %lx\n", p);

	c = p;
	c++;
	printf("addr %lx\n", c);
	printf("value %lx\n", *c);
	*(long *)c = ~0;
	printf("value %lx\n", *(long *)p);
	printf("value %lx\n", *(long *)c);
	printf("value %lx\n", *(((long *)p)+1));

	return 0;
}
