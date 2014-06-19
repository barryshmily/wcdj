#include <stdio.h>

void printIntArray(int a[], int size)
{
	for (int i = 0; i != size; ++i)
	{
		printf("%d\n", a[i]);
	}
}

int main(int argc, char **argv)
{
	int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
	//printIntArray(a, sizeof(a)/sizeof(int));

	short *p = (short *)&a[0];
	printf("*p = %d\n", *p);
	short *q = (short *)&a[1];
	printf("*q = %d\n", *q);


	short *r = (short *)a[0];
	short *s = (short *)a[1];
	printf("r = %p\n", r);
	printf("s = %p\n", s);
	printf("a[s-r] = %d\n", a[s-r]);



	short b[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
	printf("0x%x\n", *(int *)&b[0]);// 0x20001

	char c[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
	printf("0x%x\n", *(int *)&c[0]);// 0x34333231

	return 0;
}
