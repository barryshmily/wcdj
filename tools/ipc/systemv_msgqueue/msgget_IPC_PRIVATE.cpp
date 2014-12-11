#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define MAX_TEXT 512

struct my_msg_st
{
	long int my_msg_type;
	char some_text[MAX_TEXT];
};

int main(void)
{
	long int msg_to_receive=0;
	struct my_msg_st some_data;
	int msgid;
	char buffer[BUFSIZ];
	pid_t pid;

	/*创建消息队列*/
	msgid=msgget(IPC_PRIVATE,0666 | IPC_CREAT);
	if(msgid==-1)
	{
		fprintf(stderr,"msgget failed with error:%d\n",errno);
		exit(EXIT_FAILURE);
	}

	/*向消息队列中添加消息*/
	pid=fork();
	if(pid<0)
	{
		fprintf(stderr,"fork failed with error:%d\n",errno);
		exit(EXIT_FAILURE);		
	}
	else if(pid)
	{
		printf("Enter some text:");
		fgets(buffer,BUFSIZ,stdin);
		some_data.my_msg_type=1;
		strcpy(some_data.some_text,buffer);

		/*添加消息*/
		if(msgsnd(msgid,(void *)&some_data,MAX_TEXT,0)==-1)
		{
			fprintf(stderr,"msgsed failed\n");
			exit(EXIT_FAILURE);
		}

		wait(NULL);
	}
	else
	{
		sleep(2);
		/*读取消息*/
		if(msgrcv(msgid,(void *)&some_data,BUFSIZ,msg_to_receive,0)==-1)
		{
			fprintf(stderr,"msgrcv failed with error: %d\n",errno);
			exit(EXIT_FAILURE);
		}
		printf("You wrote: %s",some_data.some_text);

		/*从系统内核中移走消息队列*/
		if(msgctl(msgid,IPC_RMID,0)==-1)
		{
			fprintf(stderr,"msgctl(IPC_RMID) failed\n");
			exit(EXIT_FAILURE);
		}

	}
	return 0;
}

