#include <iostream>
#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

void* thr_func (void*)
{
	for(;;) sleep(500);
	return 0;
}

void max_thread_test ()
{
	const int MAX = 4194304;
	//const int MAX = 10;

	for (int i = 0; i < MAX; ++i) {
		int ret = 0;

		if (!(i % (MAX / 100))) std::cerr << "..." << i;

		pthread_attr_t attr;
		pthread_attr_init(&attr);

		// detached
		ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		if (ret) { 
			perror("pthread_attr_setdetachstate err"); 
			exit(1);
		};

		// thread stacksize
		int stacksize = 16384;
		pthread_attr_setstacksize(&attr, stacksize);
		if (ret) { 
			perror("pthread_attr_setstacksize err"); 
			exit(1);
		};

		pthread_t thread;
		ret = pthread_create(&thread, &attr, thr_func, NULL);
		if (ret) {
			std::cerr << "threads created " << i << std::endl ;
			perror("pthread_create");
			exit(1);
		}
	}

	std::cerr << std::endl;
	std::cout << "exit (succes)" << std::endl;
}

int main()
{
	max_thread_test();
	for(;;) sleep(5000);

	std::cout << "end" << std::endl;
	return 0;
}
// http://blog.csdn.net/delphiwcdj/article/details/53571982
