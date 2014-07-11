#include <iostream>
#include <stdio.h>
#include <unistd.h>
using namespace std;


int main()
{
	const int SIZE = 1 * 1024 * 1024;

	printf("pb beg[%lx]\n", sbrk(0));

	char * pb = new char[SIZE]();
	memset(pb, 0x31, SIZE);
	//printf("pb[%s]\n", pb);

	if (!pb)
	{
		printf("new err\n");
		return 1;
	}

	printf("pb beg[%lx]\n", sbrk(0));

	delete []pb;

	return 0;
}
