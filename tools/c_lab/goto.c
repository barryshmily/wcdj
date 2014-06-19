#include <stdio.h>

#define N 5

void loop_for()
{
	for (int i = 0; i < N; ++i)
	{
		printf("do...%d\n", i);
	}
}

void loop_goto()
{
	int i = N;

TODO:
	if (--i >= 0)
	{
		printf("do...%d\n", i);
		goto TODO;
	}
}

int main(int argc, char **argv)
{
	loop_for();

	printf("------------\n");

	loop_goto();

	return 0;
}
/*
output:
do...0
do...1
do...2
do...3
do...4
------------
do...4
do...3
do...2
do...1
do...0
 */
