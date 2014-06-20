#include <stdio.h>
#include <stdlib.h>

#define N 1000

int main(int argc, char **argv)
{
	int boy = 0;
	int girl = 0;

	srand((unsigned)time(NULL));

	for (int i = 0; i != N; ++i)
	{
		if (rand()%2 == 0)
		{
			++boy;
		}
		else
		{
			++girl;
		}
	}

	printf("boy[%d] girl[%d]\n", boy, girl);

	return 0;
}
