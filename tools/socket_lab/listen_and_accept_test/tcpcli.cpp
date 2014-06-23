#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>



int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in svraddr;

	if (argc != 2)
	{
		printf("usage: tcpcli <IPaddress>\n");
		return 0;
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&svraddr, 0x0, sizeof(svraddr));
	svraddr.sin_family = AF_INET;
	svraddr.sin_port = htons(9002);

	if (-1 == connect(sockfd, (struct sockaddr *)&svraddr, sizeof(svraddr)))
	{
		printf("connect err:%s\n", strerror(errno));
		return -1;
	}

	// do request
	sleep(60);

	return 0;
}

