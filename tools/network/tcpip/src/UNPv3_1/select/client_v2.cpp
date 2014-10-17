// client_v2.cpp  

#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <netinet/in.h>  
#include <unistd.h>  
#include <sys/socket.h>  
#include <sys/types.h>  
#include <errno.h>  


#define SERV_PORT 30008  
#define SA struct sockaddr  
#define MAXLINE 4096  
#define max(a, b) ( (a) > (b) ? (a) : (b) )  

static int read_cnt;  
static char *read_ptr;  
static char read_buf[MAXLINE];  

static ssize_t my_read(int fd, char *ptr)  
{  
	if (read_cnt <= 0)  
	{  
again:  
		if ( (read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0)  
		{  
			if (errno == EINTR)  
				goto again;  
			return -1;  
		}  
		else if (read_cnt == 0)  
			return 0;  

		read_ptr = read_buf;  
	}  

	--read_cnt;  
	*ptr = *read_ptr++;  
	return 1;  
}  

ssize_t readlinebuf(void **vptrptr)  
{  
	if (read_cnt)  
		*vptrptr = read_ptr;  

	return read_cnt;  
}  

ssize_t readline(int fd, void *vptr, size_t maxlen)  
{  
	ssize_t n, rc;  
	char c, *ptr;  

	ptr = (char *)vptr;  
	for (n = 1; n < maxlen; ++n)  
	{  
		if ( (rc = my_read(fd, &c)) == 1)  
		{  
			*ptr++ = c;  
			if (c == '\n')  
				break;// newline is stored, like fgets()  
		}  
		else if (rc == 0)  
		{  
			*ptr = 0;  
			return (n-1);// EOF, n - 1 bytes were read  
		}  
		else  
			return -1;// error, errno set by read()  
	}  

	*ptr = 0;  
	return (n);  
}  

ssize_t writen(int fd, const void *vptr, size_t n)  
{  
	size_t nleft;  
	ssize_t nwritten;  
	const char *ptr;  

	ptr = (const char*)vptr;  
	nleft = n;  
	while (nleft > n)  
	{  
		if ( (nwritten = write(fd, ptr, nleft)) <= 0 )  
		{  
			if (nwritten < 0 && errno == EINTR)  
				nwritten = 0;// and call write() again  
			else  
				return -1;// error  
		}  
		nleft -= nwritten;  
		ptr += nwritten;  
	}  
	return n;  
}  

void str_cli(FILE *fp, int sockfd)  
{  
	int maxfdp1;  
	int stdineof = 0;  

	fd_set rset;  
	char buf[MAXLINE];  
	int n;  

	FD_ZERO(&rset);  

	for ( ; ; )  
	{  
		if (stdineof == 0)  
		{  
			FD_SET(fileno(fp), &rset);  
		}  
		FD_SET(sockfd, &rset);  

		maxfdp1 = max(fileno(fp), sockfd) + 1;  

		select(maxfdp1, &rset, NULL, NULL, NULL);  

		// socket is readable  
		if (FD_ISSET(sockfd, &rset))  
		{  
			printf("socket is readable\n");  

			if ( (n = read(sockfd, buf, MAXLINE)) == 0 )  
			{  
				if (stdineof == 1)  
				{  
					printf("normal tremination\n");  

					return;// normal termination  
				}  
				else  
				{  
					printf("str_cli: server terminated prematurely\n");  
					exit(1);  
				}  
			}  
			write(fileno(stdout), buf, n);  
		}  

		// input is readable  
		if (FD_ISSET(fileno(fp), &rset))  
		{  
			printf("input is readable\n");  

			if ( (n = read(fileno(fp), buf, MAXLINE)) == 0 )  
			{  
				printf("input read 0\n");  

				stdineof = 1;  
				shutdown(sockfd, SHUT_WR);// send FIN  
				FD_CLR(fileno(fp), &rset);// turn off the bit for fileno(fp) in rset  
				continue;  
			}  
			buf[n] = '\0';  
			printf("input read:%s", buf);  
			//int iret = writen(sockfd, buf, n);  
			int iret = send(sockfd, buf, n, 0);  
			printf("socket send:%d\n", iret);  
		}  

	}  
}  

int main(int argc, char **argv)  
{  
	int sockfd;  
	struct sockaddr_in servaddr;  

	if (argc != 2)  
	{  
		printf("usage: tcpcli <IPaddress>\n");  
		exit(1);  
	}  

	sockfd = socket(AF_INET, SOCK_STREAM, 0);  
	memset(&servaddr, 0x0, sizeof(servaddr));  
	servaddr.sin_family = AF_INET;  
	servaddr.sin_port = htons(SERV_PORT);  

	if (-1 == connect(sockfd, (SA *)&servaddr, sizeof(servaddr)))
	{
		printf("connect err:%s\n", strerror(errno));  
		exit(1); 
	}

	str_cli(stdin, sockfd);  

	exit(0);  
}  
