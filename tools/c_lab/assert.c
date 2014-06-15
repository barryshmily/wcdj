// test assert

/* 在调试结束后，可以通过在#include <assert.h>语句之前插入#define NDEBUG来禁止assert调用
 * */
#define NDEBUG
#include <assert.h>
#include <stdio.h>


int main(int argc, char **argv)
{
	FILE *fp = NULL;
	fp = fopen("test.txt", "w");
	printf("1[%p]\n", fp);
	assert(fp);
	fclose(fp);

	fp = NULL;
	fp = fopen("noexitfile.txt", "r");
	printf("2[%p]\n", fp);
	assert(fp);
	fclose(fp);

	return 0;
}
/*
output:
./assert 
1[0x7fff7ba372a0]
2[0x0]
Assertion failed: (fp), function main, file assert.c, line 19.
Abort trap: 6
 */
