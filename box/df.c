#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[]) {
	int i, *ptr, *tmp;
	int *ptr1, *tmp1;

	tmp = ptr = malloc(sizeof(int) * 120); // origin 60 minimal 31
	printf("sizeof int %x tmp  %0lx ptr %0lx\n", sizeof(int), tmp, ptr);
	for (i = 0; i < 122; i ++) // origin 76; minimal 63 for 60; minimal 35 for 31.
	{
	    *ptr = i;
	    ptr++;
	}

	printf("tmp  %0lx ptr %0lx\n", tmp, ptr);
	printf(" trying to free tmp\n");
	free (tmp);
}
