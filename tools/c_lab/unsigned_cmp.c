#include <stdio.h>

int main(int argc, char **argv)
{
	unsigned int i = 1;

	// warning: comparison of unsigned expression < 0 is always false [-Wtautological-compare]
	if (i < 0)
	{
		printf("test\n");
	}

	return 0;
}
