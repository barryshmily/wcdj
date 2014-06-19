#include <stdio.h>

int main(int argc, char **argv)
{
	printf("sizeof(char) = %lu\n",      sizeof(char));
	printf("sizeof(short) = %lu\n",     sizeof(short));
	printf("sizeof(int) = %lu\n",       sizeof(int));
	printf("sizeof(long) = %lu\n",      sizeof(long));
	printf("sizeof(long long) = %lu\n", sizeof(long long));
	printf("sizeof(float) = %lu\n",     sizeof(float));
	printf("sizeof(double) = %lu\n",    sizeof(double));
	printf("sizeof(char *) = %lu\n",    sizeof(char *));
	printf("sizeof(int *) = %lu\n",     sizeof(int *));

	return 0;
}
/*
output: (Mac OS X 64bits)
sizeof(char) = 1
sizeof(short) = 2
sizeof(int) = 4
sizeof(long) = 8
sizeof(long long) = 8
sizeof(float) = 4
sizeof(double) = 8
sizeof(char *) = 8
sizeof(int *) = 8

 */
