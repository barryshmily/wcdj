#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdlib.h>

#define SERV_PORT 30009
#define MAXLINE 1024 

void handle(FILE *fp, int sockfd)
{
	int maxfdp1 = 0;
	fd_set rset;
	char sendline[MAXLINE] = "1234abcdefgh11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111"; 
	char recvline[MAXLINE];
	int tmpfd;


	FD_ZERO(&rset);

	for( ; ; ) 
	{
		//printf("fileno(fp)=%d\n", fileno(fp));
		//FD_SET(fileno(fp), &rset);

		FD_ZERO(&rset);

		printf("sockfd=%d\n", sockfd);
		FD_SET(sockfd, &rset);
		maxfdp1 = sockfd;


		printf("11111111111\n");

		//select(maxfdp1+1, &rset, NULL, NULL, NULL);
		int iret = select(maxfdp1+1, NULL, &rset, NULL, NULL);
		if (iret < 0)
		{
			printf("select error!\n");
		}
		else if (iret == 0)
		{
			printf("select timeout!\n");
		}

		printf("22222222222\n");

		if (FD_ISSET(sockfd, &rset))
		{
			printf("socket is readable\n");
			write(sockfd, sendline, strlen(sendline));

		}
		/*if (FD_ISSET(fileno(fp), &rset))
		  {
		  printf("input is readable\n");
		  if (fgets(sendline, MAXLINE, fp) == NULL)
		  return;
		  write(sockfd, sendline, strlen(sendline));
		  }*/

	}
}

int main(int argc, char** argv)
{
	int i, sockfd[2000];
	struct sockaddr_in servaddr;

	for (i = 0; i != 1030; ++i) {
		sockfd[i] = socket(AF_INET, SOCK_STREAM, 0);
		memset(&servaddr, 0x0, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(SERV_PORT);
		inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

		// set non-block
		int val;
		if ((val = fcntl(sockfd[i], F_GETFL, 0)) == -1) {
			printf("set non-block err!\n");
		}
		if (fcntl(sockfd[i], F_SETFL, val | O_NONBLOCK) == -1) {
			printf("set non-block err2!\n");
		}

		int iret = connect(sockfd[i], (struct sockaddr*)&servaddr, sizeof(servaddr));
		if (iret < 0) continue;
		printf("sockfd[%d]=%d\n", i, sockfd[i]);
	}

	if (sockfd[1029] != -1)
		handle(stdin, sockfd[1029]);

	printf("done\n");
	exit(0);

}

