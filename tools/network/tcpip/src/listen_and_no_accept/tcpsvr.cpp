#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>


int main(int argc, char **argv)
{
	int listenfd;
	int connfd;
	int sockfd;

	struct sockaddr_in cliaddr;
	struct sockaddr_in svraddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&svraddr, 0x0, sizeof(svraddr));
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	svraddr.sin_port = htons(9002);

	bind(listenfd, (struct sockaddr *)&svraddr, sizeof(svraddr));
	listen(listenfd, 0);

	// no accept
	for(;;)
		;

	return 0;
}

