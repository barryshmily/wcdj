// fifo3.c

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
#define MY_MSG_SIZE_BIG (1024 * 1024 * 1024 * 1)// 1GB
#define MY_MSG_SIZE_SMALL (1024 * 16)// 16KB


int main(int argc, char **argv)
{

	int pipe_fd;
	int res;
	int open_mode = O_WRONLY;
	int bytes_sent = 0;
	char buffer[BUFFER_SIZE + 1];
	
	
	if (access(FIFO_NAME, F_OK) == -1)
	{
		res = mkfifo(FIFO_NAME, 0777);
		if (res != 0)
		{
			fprintf(stderr, "mkfifo[%s] error[%d:%s]\n", FIFO_NAME, errno, strerror(errno));
			return -1;
		}
	}

	printf("Process[%d] opening FIFO[%s] O_WRONLY\n", getpid(), FIFO_NAME);
	pipe_fd = open(FIFO_NAME, open_mode);
	printf("Process[%d] open result[%d]\n", getpid(), pipe_fd);


	if (pipe_fd != -1)
	{
		//while (bytes_sent < MY_MSG_SIZE_SMALL) 
		while (bytes_sent < MY_MSG_SIZE_BIG) 
		{
			res = write(pipe_fd, buffer, BUFFER_SIZE);
			if (res == -1)
			{
				fprintf(stderr, "Process[%d] write[%d] error on pipe[%d:%s]\n", getpid(), bytes_sent, errno, strerror(errno));
				return -1;
			}
			bytes_sent += res;
		}

		close(pipe_fd);
	}

	printf("Process[%d] finished\n", getpid());

	return 0;
}
// gcc -o fifo3 fifo3.c
