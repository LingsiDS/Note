#include <stdio.h>

int main () {
	const int a = 10;
	int *p = (int *)&a;
	printf("a = %d\n", a);
	printf("*p = %d\n", *p);
	printf("&a = %p, p = %p\n", &a, p);

	*p = 20;
	printf("a = %d\n", a);
	printf("*p = %d\n", *p);	
	printf("&a = %p, p = %p\n", &a, p);

	
}