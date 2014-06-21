#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>// STDIN_FILENO
#include <string.h>

int main(int argc, char **argv)
{
	// open existing file for reading
	int fd = open("test.txt", O_RDONLY);
	if (fd == -1)
	{
		printf("open err\n");
		return -1;
	}
	else
	{
		printf("fd=%d\n", fd);
	}

	// open new or existing file for reading and writing, truncating to zero bytes
	// file permissions read+write for owner, nothing for all others
	fd = open("myfile", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		printf("open err\n");
		return -1;
	}
	else
	{
		printf("fd=%d\n", fd);
	}
	char buf[128] = "hello gerry!";
	//char *buf = "hello gerry!";
	//if (write(fd, buf, 128) != 128)
	if (write(fd, buf, strlen(buf)) != strlen(buf))
	{
		printf("write err\n");
		return -1;
	}
	close(fd);

	// open new or existing file for writting
	// wirtes should always append to end of file
	fd = open("w.log", O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		printf("open err\n");
		return -1;
	}
	else
	{
		printf("fd=%d\n", fd);
	}

	// SUSv3规定, 如果调用open()成功，必须保证其返回值为进程未使用文件描述符中数值最小者
	// 可以利用该特性以特定文件描述符打开某一文件
	if (close(STDIN_FILENO) == -1)
	{
		printf("close err\n");
		return -1;
	}
	int fd_stdin = open("myfile", O_RDONLY);
	if (fd_stdin == -1)
	{
		printf("open err\n");
		return -1;
	}
	else
	{
		printf("fd_stdin=%d\n", fd_stdin);
	}
	
	// still open stdin
	
	if (close(STDOUT_FILENO) == -1)
	{
		printf("close err\n");
		return -1;
	}

	int fd_stdout = open("myfile2", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd_stdout == -1)
	{
		printf("open err\n");
		return -1;
	}
	else
	{
		printf("fd_stdout=%d\n", fd_stdout);
	}
	snprintf(buf, sizeof(buf), "try to write info to stdout!");
	if (write(fd_stdout, buf, strlen(buf)) != strlen(buf))
	{
		printf("write err\n");
		return -1;
	}
	close(fd_stdin);
	close(fd_stdout);

	return 0;
}
/*
output:
fd=3
fd=4
fd=4
fd=0
 */
