#include <stdio.h>

int main()
{
	const int v2 = 100;
	int v1 = v2;

	int *p1 = &v1;
	int &r1 = v1;
	printf("*p1[%d] r1[%d]\n", *p1, r1);
	
	v1 = 200;
	printf("r1[%d]\n", r1);

	r1 = v2;
	printf("v1[%d]\n", v1);

	return 0;
}
