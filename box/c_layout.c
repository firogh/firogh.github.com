#include <stdio.h>
#include <stdlib.h>

extern char etext, edata, end;

int i = 42;

int main(int argc, char* argv[])
{
        static int j;
        printf ("end of program (etext) = %10p\n",&etext);
        printf ("end of initialized data (edata) = %10p\n",&end);
        printf ("end of uninitialized data (end) = %10p\n",&edata);
        printf ("=====\n");
        printf ("Value of i (initialized global) : %d\n",i);
        printf ("Address of i : %10p\n",&i);
        printf ("Value of j (static with no explicit initialization) : %d\n",j);
        printf ("Address of i : %10p\n",&j);
        return 0;
}
