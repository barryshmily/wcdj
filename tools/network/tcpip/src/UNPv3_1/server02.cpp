#include  <unistd.h>
#include  <sys/types.h>       /* basic system data types */
#include  <sys/socket.h>      /* basic socket definitions */
#include  <netinet/in.h>      /* sockaddr_in{} and other Internet defns */
#include  <arpa/inet.h>       /* inet(3) functions */

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define MAXLINE 1024*15 
//typedef struct sockaddr  SA;
void handle(int connfd);

int  main(int argc, char **argv)
{
	int listenfd, connfd;
	int serverPort = 30008;
	//int listenq = 1024;
	int listenq = 2048;
	pid_t   childpid;
	char buf[MAXLINE];
	socklen_t socklen;

	struct sockaddr_in cliaddr, servaddr;
	socklen = sizeof(cliaddr);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(serverPort);
	

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0) {
		perror("socket error");
		return -1;
	}
	int iReuseAddrFlag = 1;
	int ret = setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,(char*)&iReuseAddrFlag,sizeof(iReuseAddrFlag));
	printf("ret=%d\n",ret);
	if (bind(listenfd, (struct sockaddr *) &servaddr, socklen) < 0) {
		perror("bind error");
		return -1;
	}
	if (listen(listenfd, listenq) < 0) {
		perror("listen error");    
		return -1;
	}

	printf("echo server startup,listen on port:%d\n", serverPort);
	for ( ; ; )  {
		
#if 1

		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &socklen);
		printf("accept %d\n",connfd);
		if (connfd < 0) {
			perror("accept error");
			continue;
		}

		sprintf(buf, "accept form %s:%d\n", inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port);
		printf(buf,"");

		childpid = fork();
		if (childpid == 0) { 
			close(listenfd);    
			handle(connfd);   
			exit (0);
		} else if (childpid > 0)  {
			close(connfd);          
		} else {
			perror("fork error");
		}
#endif
	
		usleep(10);

	}
}


void handle(int connfd)
{
	size_t n;
	char    buf[MAXLINE] = {0};

	for(;;) {

		memset(buf, 0x0, sizeof(buf));

		n = read(connfd, buf, sizeof(buf));
		if (n < 0) {
			if(errno != EINTR) {
				perror("read error");
				break;
			}
		}
		if (n == 0) {
			//connfd is closed by client
			close(connfd);
			printf("client exit abnormally\n");
			break;
		}
		//client exit
		if (strncmp("exit", buf, 4) == 0) {
			close(connfd);
			printf("client exit normally\n");
			break;
		}
		printf("%s\n", buf+4);


		// echo, gerryyang
		//snprintf(buf,sizeof(buf),"%d",1234);
		write(connfd, buf, sizeof(buf)); //write maybe fail,here don't process failed error

		//::shutdown(connfd,SHUT_RDWR);
		//char* p = NULL;
		//memcpy(p,buf,sizeof(buf));

	} 
}

