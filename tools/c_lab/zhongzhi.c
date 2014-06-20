// #define middle(a,b,c)  实现求中值的宏

#include <stdio.h>

#define MAX(a, b, c) (a > b ? (a > c ? a : c) : (b > c ? b : c))
#define MIN(a, b, c) (a < b ? (a < c ? a : c) : (b < c ? b : c))
// error, maybe overflow!
//#define MIDDLE(a, b, c) (a + b + c - MAX(a, b, c) - MIN(a, b, c))
#define MIDDLE(a, b, c) (a > b ? (b > c ? b : (a > c ? c : a)) : (b < c ? b : (a > c ? a : c)))

int main(int argc, char **argv)
{
	// test
	printf("MIDDLE(1, 2, 3) = %d\n", MIDDLE(1, 2, 3));
	printf("MIDDLE(1, 3, 2) = %d\n", MIDDLE(1, 3, 2));
	printf("MIDDLE(3, 2, 1) = %d\n", MIDDLE(3, 2, 1));
	printf("MIDDLE(3, 1, 2) = %d\n", MIDDLE(3, 1, 2));
	printf("MIDDLE(3, 7, 5) = %d\n", MIDDLE(3, 7, 5));

	return 0;
}
/*
output:
MIDDLE(1, 2, 3) = 2
MIDDLE(1, 3, 2) = 2
MIDDLE(3, 2, 1) = 2
MIDDLE(3, 1, 2) = 2
MIDDLE(3, 7, 5) = 5
 */
