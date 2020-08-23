#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <pthread.h> 
  
int g = 0; 
int atomic_g = 0;
int g1 = 0;
  
void *thread_foo(void *vargp) 
{ 
	int *myid = (int *)vargp; 
	int l = 0;
	int inc = 1;

	while (l <= 500) {
		++l; 
		++g; 
		asm volatile( "lock; incl %0":"+m" (atomic_g));
		asm volatile( "incl %0":"+m" (g1));
	}

	printf("TID: %x, Local: %d, Global: %d, %d, %d \n", *myid, ++l, ++g, atomic_g, g1); 

} 
  
int main() 
{ 
	int i; 
	pthread_t tid[2]; 
  
	for (i = 0; i < 2; i++) 
		pthread_create(&tid[i], NULL, thread_foo, (void *)&tid); 

	for (int i = 0; i < 2; i++) {
		pthread_join(tid[i], NULL);  
	}
 
	return 0; 
} 
