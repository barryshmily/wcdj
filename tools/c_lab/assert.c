// test assert

#include <assert.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	FILE *fp = NULL;
	fp = fopen("test.txt", "w");
	assert(fp);
	fclose(fp);

	fp = fopen("noexitfile.txt", "r");
	assert(fp);
	fclose(fp);

	return 0;
}
/*
output:
./assert 
Assertion failed: (fp), function main, file assert.c, line 14.
Abort trap: 6
 */
