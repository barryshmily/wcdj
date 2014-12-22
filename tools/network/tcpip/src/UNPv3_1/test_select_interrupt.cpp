#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <signal.h>

#define STDIN 0

static void sigHandler(int sig)
{
	if (sig == SIGINT)
	{
		printf("caught SIGINT\n");
		return;
	}

	printf("caught SIGQUIT\n");
	exit(EXIT_SUCCESS);
}

int main()
{
	if (signal(SIGINT, sigHandler) == SIG_ERR)
	{
		printf("signal err\n");
	}

	if (signal(SIGQUIT, sigHandler) == SIG_ERR)
	{
		printf("signal err\n");
	}


	int ret = 0;
	int maxfdp1 = STDIN;
	fd_set rset;
	FD_ZERO(&rset);
	FD_SET(STDIN, &rset);

#if 0
	char ch;
	scanf("%c\n", &ch);
	printf("ch[%c]\n", ch);
#endif

	while (1)
	{
		if ((ret = select(maxfdp1 + 1, &rset, NULL, NULL, NULL)) > 0)
		{
			if (FD_ISSET(STDIN, &rset))
			{
				printf("STDIN readable\n");

				// simulate to consume it
				sleep(1);
				// 如果clear后下次select还需要set, 否则无法监听
				//FD_CLR(STDIN, &rset);
			}
		}
		else
		{
			if (errno == EINTR)
			{
				printf("select: EINTR return[%d]\n", ret);
				continue;
			}
			else if (ret == 0)
			{
				printf("select: timeout return[%d]\n", ret);
				break;
			}
			else
			{
				printf("select: err[%d:%s] return[%d]\n", errno, strerror(errno), ret);
				break;
			}
		}
	}

	printf("exit\n");
	return 0;
}
