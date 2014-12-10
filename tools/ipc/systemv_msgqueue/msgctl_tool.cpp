/* 
 * function: delete System V message queue that has 0x00000000 msgkey
 * */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		printf("usage: %s cmd msqid\ncmd: 0 IPC_STAT\t1 IPC_RMID\n", argv[0]);
		exit(0);
	}

	int cmd   =  atoi(argv[1]);
	int msqid =  atoi(argv[2]);
	struct msqid_ds ds;

	int ret = 0;
	if ((ret = msgctl(msqid, (cmd == 0 ? IPC_STAT : IPC_RMID), (struct msqid_ds *)&ds)) < 0)
	{
		printf("msgctl err[%d:%s]\n", errno, strerror(errno));
		return -1;
	}

	if (cmd == 0)
		printf("msqid_ds.msg_lspid[%u] msqid_ds.msg_lrpid[%u]\n", 
				ds.msg_lspid,
				ds.msg_lrpid);

	return 0;
}
/*
# del 0x00000000
ipcs -q | grep gerryyang | awk '{print $2}' | while read f;do ./msgctl_tool 1 $f;done
 
 */
