// fifo1.c

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>


int main()
{
	int res = mkfifo("/tmp/my_fifo", 0777);
	if (res == 0)
	{
		printf("FIFO created\n");
	}
	else if (errno = EEXIST)
	{
		printf("/tmp/my_fifo has exsited!\n");
	}
	else
	{
		printf("mkfifo error[%d:%s]\n", errno, strerror(errno));
	}

	return 0;
}
// gcc -o fifo1 fifo1.c
