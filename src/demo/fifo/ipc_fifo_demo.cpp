#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <string>
#include <pthread.h>
#include "ipc_fifo_notify.h"
using namespace std;

void *thread_function_client(void *arg);
void *thread_function_svr(void *arg);

char msg[] = "gerryyang";

string g_path_fifo_name = "fifo_file";

int main(int argc, char **argv)
{
	CNotify _fifo_client;
	CNotify _fifo_svr;

	// -----------------------------------------------
	// create threads
	int res = 0;
	pthread_t thread_client;
	pthread_t thread_svr;
	void *thread_client_result;
	void *thread_svr_result;

	res = pthread_create(&thread_client, NULL, thread_function_client, (CNotify *)&_fifo_client);
	if (res != 0) {
		perror("pthread_create client error\n");
		return -1;
	}

	res = pthread_create(&thread_svr, NULL, thread_function_svr, (CNotify *)&_fifo_svr);
	if (res != 0) {
		perror("pthread_create svr error\n");
		return -1;
	}

	printf("waiting for threads to finish...\n");
	res = pthread_join(thread_client, &thread_client_result);
	if (res != 0) {
		perror("pthread_join client error\n");
		return -1;
	} else {
		printf("pthread_join client ok, it returned [%s]\n", (char *)thread_client_result);
	}

	res = pthread_join(thread_svr, &thread_svr_result);
	if (res != 0) {
		perror("pthread_join svr error\n");
		return -1;
	} else {
		printf("pthread_join svr ok, it returned [%s]\n", (char *)thread_svr_result);
	}

	return 0;
}

void *thread_function_client(void *arg)
{
	printf("thread_function_client is running\n");

	CNotify *p_fifo_client = (CNotify *)arg;
	p_fifo_client->open(g_path_fifo_name.c_str(), false);// true-read false-write

	for (;;)
	{
		sleep(1);

		try
		{
			p_fifo_client->set_notify();
		}
		catch (runtime_error& e)
		{
			cerr << "catch exception: " << e.what() << "\n" << endl;
			pthread_exit((char *)"client: thread_function_client exception");
		}

		printf("client: send a msg\n");
	}

	pthread_exit((char *)"client: Thank you for the CPU time");
}

void *thread_function_svr(void *arg)
{
	printf("thread_function_svr is running\n");

	CNotify *p_fifo_svr = (CNotify *)arg;
	p_fifo_svr->open(g_path_fifo_name.c_str(), true);// true-read false-write
	char buffer[1024] = {0};

	for (;;)
	{
		try
		{
			p_fifo_svr->get_notify(buffer, sizeof(buffer));
		}
		catch (runtime_error& e)
		{
			cerr << "catch exception: " << e.what() << "\n" << endl;
			pthread_exit((char *)"client: thread_function_client exception");
		}

		printf("svr: receive a msg[%s]\n", buffer);
		memset(buffer, 0x0, sizeof(buffer));
	}

	pthread_exit((char *)"svr: Thank you for the CPU time");
}

