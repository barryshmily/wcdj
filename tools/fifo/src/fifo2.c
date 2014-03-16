// fifo2.c

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>


#define FIFO_NAME "/tmp/my_fifo"

int main(int argc, char **argv)
{

	int res;
	int open_mode = 0;
	int i;

	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s <some combination of O_RDONLY O_WRONLY O_NONBLOCK>\n", argv[0]);
		return -1;
	}

	int iFind = 0;
	for (i = 1; i < argc; ++i)
	{
		if (strncmp(*++argv, "O_RDONLY",   8)  == 0)  { open_mode |= O_RDONLY;   iFind = 1; }
		if (strncmp(*argv,   "O_WRONLY",   8)  == 0)  { open_mode |= O_WRONLY;   iFind = 1; }
		if (strncmp(*argv,   "O_NONBLOCK", 10) == 0)  { open_mode |= O_NONBLOCK; iFind = 1; }
	}
	if (iFind == 0)
	{
		fprintf(stderr, "invalid para, Usage: %s <some combination of O_RDONLY O_WRONLY O_NONBLOCK>\n", argv[0]);
		return -1;
	}

	
	if (access(FIFO_NAME, F_OK) == -1)
	{
		int res = mkfifo(FIFO_NAME, 0777);
		if (res != 0)
		{
			fprintf(stderr, "mkfifo[%s] error[%d:%s]\n", FIFO_NAME, errno, strerror(errno));
			return -1;
		}
	}

	printf("Process[%d] opening FIFO[%s]\n", getpid(), FIFO_NAME);
	res = open(FIFO_NAME, open_mode);
	printf("Process[%d] open result[%d]\n", getpid(), res);

	sleep(5);

	if (res != -1) 
	{
		close(res);
	}

	printf("Process[%d] finished\n", getpid());

	return 0;
}
// gcc -o fifo2 fifo2.c
