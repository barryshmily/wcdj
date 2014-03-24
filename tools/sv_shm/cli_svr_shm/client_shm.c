// client_shm.c

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
	int shmid;

	srand((unsigned int)getpid());
	shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 06666 | IPC_CREAT);
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
	shared_stuff->written_by_you = 0;
	while (running) {
		if (shared_stuff->written_by_you) {
			printf("You wrote: %s", shared_stuff->some_text);
			sleep(rand() % 4);// make the other process wait for us
			shared_stuff->written_by_you = 0;
			if (strncmp(shared_stuff->some_text, "end", 3) == 0) {
				running = 0;
			}
		}
	}

	// detach shm
	// 将共享内存分离但并未删除它, 只是使得该共享内存对当前进程不再可用
	if (shmdt(shared_memory) == -1) {
		fprintf(stderr, "shmdt error[%d:%s]\n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	if (shmctl(shmid, IPC_RMID, 0) == -1) {
		fprintf(stderr, "shmctl error[%d:%s]\n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	exit(EXIT_FAILURE);
}
// gcc -o client_shm client_shm.c 

