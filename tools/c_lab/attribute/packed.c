#include <stdio.h>

struct unpack
{
	char s;
	int i;
};

struct pack
{
	char s;
	int i;
} __attribute__((__packed__));

int main()
{
	printf("size of int is %lu\n", sizeof(int));
	printf("size of char is %lu\n", sizeof(char));

	printf("size of pack is %lu and unpack is %lu\n",
			sizeof(struct pack), sizeof(struct unpack));

	return 0;
}
/*
output:
gerryyang@mba:attribute$./packed 
size of int is 4
size of char is 1
size of pack is 5 and unpack is 8
 */
