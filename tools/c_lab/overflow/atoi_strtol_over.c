#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

// 大数计算可以使用python校验, 可以避免溢出的问题
// 测试环境: OS X 64bits

int main()
{
	/*
http://www.cplusplus.com/reference/cstdlib/atoi/
On success, the function returns the converted integral number as an int value.
If the converted value would be out of the range of representable values by an int, it causes undefined behavior. See strtol for a more robust cross-platform alternative when this is a possibility.
	 * */

	// undefined behavior
	char *a = "2147483648";
	int ia = atoi(a);
	printf("atoi(a) = %d\n", ia);// -2147483648

	char *b = "-2147483647";
	int ib = atoi(b);
	printf("atoi(b) = %d\n", ib);// -2147483647 

	char *c = "3.14";
	int ic = atoi(c);
	printf("atoi(c) = %d\n", ic);// 3


	/*
http://www.cplusplus.com/reference/cstdlib/strtol/
On success, the function returns the converted integral number as a long int value.
If no valid conversion could be performed, a zero value is returned (0L).
If the value read is out of the range of representable values by a long int, the function returns LONG_MAX or LONG_MIN (defined in <climits>), and errno is set to ERANGE.
	 * */
	char *d = "100000000000000000000";// 2^63 = 10^18*8
	char *pEnd;
	long id = strtol(d, &pEnd, 10);
	if (id == LONG_MAX || id == LONG_MIN)
	{
		printf("strtol err[%d:%s]\n", errno, strerror(errno));
	}
	else
	{
		printf("strtol(d, &pEnd, 10) = %ld\n", id);
	}
	
	long ie = 2147483647 * 2147483647;
	printf("2147483647 * 2147483647 = %ld\n", ie);// 1

	long iff = 2147483647L * 2147483647;
	printf("2147483647L * 2147483647 = %ld\n", iff);// 4611686014132420609

	long ig = (long)2147483647 * 2147483647;
	printf("(long)2147483647 * 2147483647 = %ld\n", ig);// 4611686014132420609

	return 0;
}
