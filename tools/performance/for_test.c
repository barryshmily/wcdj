#include <stdio.h>

int fun1()
{
	int sum = 0;
	for (unsigned long long row = 0; row < 10000000ULL; ++row)
	{
		for (unsigned long long col = 0; col < 5; ++col)
		{
			sum += 1;
		}
	}
	return sum;
}

int fun2()
{
	int sum = 0;
	for (int col = 0; col < 5; ++col)
	{
		for (unsigned long long row = 0; row < 10000000ULL; ++row)
		{
			sum += 1;
		}
	} 
	return sum;
}

int main()
{
	printf("fun1=%d\n", fun1());
	//printf("fun2=%d\n", fun2());
	return 0;
}

// gcc -std=c99 -o for_test for_test.c
// 在多重循环中，如果有可能，应当将最长的循环放在最内层，最短的循环放在最外层，以减少CPU 跨切循环层的次数
