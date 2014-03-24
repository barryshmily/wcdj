// svr_shm.c

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>

#define TEXT_SZ 2048
struct shared_use_st 
{
	int written_by_you;
	char some_text[TEXT_SZ];
};

int main(int argc, char **argv)
{
	int running = 1;
	void *shared_memory = (void *)0;
	struct shared_use_st *shared_stuff;
	char buffer[BUFSIZ];
	int shmid;

	shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);
	if (shmid == -1) {
		fprintf(stderr, "shmget error[%d:%s]\n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	shared_memory = shmat(shmid, (void *)0, 0);
	if (shared_memory == (void *)-1) {
		fprintf(stderr, "shmat error[%d:%s]\n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	printf("Memory attached at %X\n", (unsigned int)shared_memory);

	shared_stuff = (struct shared_use_st *)shared_memory;
	while (running) {
		while (shared_stuff->written_by_you == 1) {
			sleep(1);
			printf("waiting for client...\n");
		}
		printf("Enter some text: ");
		fgets(buffer, BUFSIZ, stdin);
		strncpy(shared_stuff->some_text, buffer, TEXT_SZ);
		shared_stuff->written_by_you = 1;
		
		if (strncmp(buffer, "end", 3) == 0) {
			running = 0;
		}
	}

	// detach shm
	// 将共享内存分离但并未删除它, 只是使得该共享内存对当前进程不再可用
	if (shmdt(shared_memory) == -1) {
		fprintf(stderr, "shmdt error[%d:%s]\n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
// gcc -o svr_shm svr_shm.c

