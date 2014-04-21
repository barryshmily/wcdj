#include <iostream>
#include <stdarg.h>
using namespace std;

const int N = 5;

void varg_test(int fmt, ...);


int main(int argc, char **argv)
{
	varg_test(0, 2, 4, 6, 8);

	return 0;
}

void varg_test(int fmt, ...)
{
	va_list ap;
	int arr[N];

	va_start(ap, fmt);

	arr[0] = fmt;

	for (int i = 1; i < N; ++i)
	{
		arr[i] = va_arg(ap, int);
	}

	va_end(ap);

	for (int i = 0; i != N; ++i)
	{
		cout << arr[i] << " ";
	}
	cout << endl;

	return;
}
/*
output:
0 2 4 6 8 
*/
