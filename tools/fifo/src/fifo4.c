// fifo4.c

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>


#define FIFO_NAME "/tmp/my_fifo"
#define BUFFER_SIZE PIPE_BUF

int main(int argc, char **argv)
{

	int pipe_fd;
	int res;
	int open_mode = O_RDONLY;
	int bytes_read = 0;
	char buffer[BUFFER_SIZE + 1] = {0};
	
	
	if (access(FIFO_NAME, F_OK) == -1)
	{
		res = mkfifo(FIFO_NAME, 0777);
		if (res != 0)
		{
			fprintf(stderr, "mkfifo[%s] error[%d:%s]\n", FIFO_NAME, errno, strerror(errno));
			return -1;
		}
	}

	printf("Process[%d] opening FIFO[%s] O_RDONLY\n", getpid(), FIFO_NAME);
	pipe_fd = open(FIFO_NAME, open_mode);
	printf("Process[%d] open result[%d]\n", getpid(), pipe_fd);


	if (pipe_fd != -1)
	{
		do 
		{
			res = read(pipe_fd, buffer, BUFFER_SIZE);
			bytes_read += res;
		} while (res > 0);
			
		close(pipe_fd);
	}
	else
	{
		fprintf(stderr, "open fifo[%s] error[%d:%s]\n", FIFO_NAME, errno, strerror(errno));
	}

	printf("Process[%d] finished\n", getpid());

	return 0;
}
// gcc -o fifo4 fifo4.c
