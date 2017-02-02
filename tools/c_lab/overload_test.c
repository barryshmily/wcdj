// http://www.cplusplus.com/reference/cstdlib/qsort/
/* qsort example */
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* qsort */

int values[] = { 40, 10, 100, 90, 20, 25 };

int compare (const void * a, const void * b)
{
	return ( *(int*)a - *(int*)b );
}

void fun()
{
	printf("fun()\n");
}

/*
$gcc -o overload_test overload_test.c 
overload_test.c:18:6: error: redefinition of 'fun'
void fun(int a)
     ^
overload_test.c:13:6: note: previous definition is here
void fun()
     ^
1 error generated.
 */
#if 0
void fun(int a)
{
	printf("fun(int a)\n");
}
#endif

int main ()
{
	// 测试C语言是否支持overload重载
	fun();

	// C语言可以通过不同的函数指针来模拟overload重载
	int n;
	qsort (values, 6, sizeof(int), compare);
	for (n=0; n<6; n++)
		printf ("%d ",values[n]);

	return 0;
}
