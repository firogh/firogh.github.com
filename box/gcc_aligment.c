#include <stdio.h>

struct S1 { short s1; unsigned long l1; short s2; short s3;char c1; char c2;}__attribute__ ((aligned(4))); 
struct S2 { char c1;} __attribute__ ((aligned )); 
struct S3 { long c1;};
struct S4 { char c1; struct S3 s3;};
int main(void)
{
        struct S1 s;
        printf("%d\n", __alignof__(s));
        printf("%d\n", __alignof__(s.s1));
        printf("%d\n", __alignof__(s.l1));
        printf("%d\n", __alignof__(s.s2));
        printf("%d\n", __alignof__(s.s3));
        printf("%d\n", __alignof__(s.c1));
        printf("%d\n", __alignof__(s.c2));
        struct S2 s2; 
        printf("%d\n", __alignof__(s2));
        printf("%d\n", __alignof__(s2.c1));
        struct S3 s3; 
        printf("%d\n", __alignof__(s3));
        struct S4 s4; 
        printf("%d\n", __alignof__(s4));
        printf("%d\n", __alignof__(s4.c1));
        printf("%d\n", __alignof__(s4.s3));
        return 0;
}

