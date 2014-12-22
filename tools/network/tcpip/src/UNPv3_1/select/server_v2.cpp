// server_v2.cpp  
// 使用单进程和select的TCP服务器  
  
#include <stdio.h>  
#include <stdlib.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <sys/select.h>  
#include <sys/time.h>  
#include <string.h>  
#include <unistd.h>  
#include <errno.h>  
  
#define SERV_PORT 30008  
#define SA struct sockaddr  
#define MAXLINE 4096  
#define max(a, b) ( (a) > (b) ? (a) : (b) )  
#define LISTENQ 1024  
  
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
  
int   
main(int argc, char **argv)  
{  
    int i;  
    int maxi;  
    int maxfd;  
    int listenfd;  
    int connfd;  
    int sockfd;  
  
    int nready, client[FD_SETSIZE];  
    ssize_t n;  
    fd_set rset, allset;  
    char buf[MAXLINE];  
    socklen_t clilen;  
    struct sockaddr_in cliaddr, servaddr;  
  
    listenfd = socket(AF_INET, SOCK_STREAM, 0);  
  
    memset(&servaddr, 0x0, sizeof(servaddr));  
    servaddr.sin_family = AF_INET;  
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);  
    servaddr.sin_port = htons(SERV_PORT);  
  
    bind(listenfd, (SA *)&servaddr, sizeof(servaddr));  
  
    listen(listenfd, LISTENQ);  
  
    maxfd = listenfd;  // initialize  
    maxi = -1;          // index into client[] array  
    for (i = 0; i < FD_SETSIZE; ++i)  
    {  
        client[i] = -1;// -1 indicates available entry  
    }  
    FD_ZERO(&allset);  
    FD_SET(listenfd, &allset);  
  
    for ( ; ; )  
    {  
        rset = allset;// structure assignment  
        nready = select(maxfd+1, &rset, NULL, NULL, NULL);  
        printf("select ready:%d\n", nready);  
  
        // new client connection  
        if (FD_ISSET(listenfd, &rset))  
        {  
            printf("new client connection\n");  
  
            clilen = sizeof(cliaddr);  
            connfd = accept(listenfd, (SA *)&cliaddr, &clilen);  
  
            for(i = 0; i < FD_SETSIZE; ++i)  
            {  
                if (client[i] < 0)  
                {  
                    client[i] = connfd;// save descriptor  
                    break;  
                }  
            }  
            if (i == FD_SETSIZE)  
            {  
                printf("too many clients\n");  
                exit(1);  
            }  
  
            FD_SET(connfd, &allset);// add new descriptor to set  
            if (connfd > maxfd)  
            {  
                maxfd = connfd;// for select  
            }  
            if (i > maxi)  
            {  
                maxi = i;// max index in client[] array  
            }  
            if (--nready <= 0)  
            {  
                printf("listen, no more readable descriptors\n");  
                continue;// no more readable descriptors  
            }  
        }  
  
        // check all clients for data  
        printf("check all clients for data\n");  
        for (i = 0; i<= maxi; ++i)  
        {  
            if ( (sockfd = client[i]) < 0 )  
            {  
                continue;  
            }  
            if (FD_ISSET(sockfd, &rset))  
            {  
                if ( (n = read(sockfd, buf, MAXLINE)) == 0 )  
                {  
                    // connection closed by client  
                    printf("connection closed by client\n");  
  
                    close(sockfd);  
                    FD_CLR(sockfd, &allset);  
                    client[i] = -1;  
                }  
                else  
                {  
                    buf[n] = '\0';  
                    printf("get info:%s", buf);  
                    writen(sockfd, buf, n);  
                }  
  
                if (--nready <= 0)  
                    break;// no more readable descriptors  
            }  
        }  
    }  
  
}
