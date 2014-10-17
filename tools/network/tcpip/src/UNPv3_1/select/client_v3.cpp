// client_v3.cpp  

#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <netinet/in.h>  
#include <unistd.h>  
#include <sys/socket.h>  
#include <sys/types.h>  
#include <errno.h>  
#include <arpa/inet.h>


#define SERV_PORT 30008  
#define SA struct sockaddr  
#define MAXLINE 4096  
#define max(a, b) ( (a) > (b) ? (a) : (b) )  



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
/*
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
*/

	if (argc != 2)  
	{  
		printf("usage: tcpcli <IPaddress>\n");  
		exit(1);  
	} 

	int i, sockfd[2000];
	struct sockaddr_in servaddr;

	for (i = 0; i != 1030; ++i) {
		sockfd[i] = socket(AF_INET, SOCK_STREAM, 0);
		memset(&servaddr, 0x0, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(SERV_PORT);
		inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

		// set non-block
		/*int val;
		if ((val = fcntl(sockfd[i], F_GETFL, 0)) == -1) {
			printf("set non-block err!\n");
		}
		if (fcntl(sockfd[i], F_SETFL, val | O_NONBLOCK) == -1) {
			printf("set non-block err2!\n");
		}*/

		int iret = connect(sockfd[i], (struct sockaddr*)&servaddr, sizeof(servaddr));
		if (iret < 0) 
		{
			printf("sockfd[%d]=%d connect err:%s\n", i, sockfd[i], strerror(errno));
			continue;
		}
		printf("sockfd[%d]=%d connected\n", i, sockfd[i]);
	}

	if (sockfd[1026] != -1)
	{
		printf("sockfd[1026]=%d\n", sockfd[1026]);
		str_cli(stdin, sockfd[1026]);  
	}

	printf("deal over\n");
	exit(0);  
}  
