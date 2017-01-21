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

	int i = 0;
	for (; i < MAX; ++i) {

		int ret = 0;
		pthread_attr_t attr;
		pthread_attr_init(&attr);

		// detached
		ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		if (ret) { 
			perror("pthread_attr_setdetachstate err"); 
			exit(1);
		}

		size_t stacksize = 0;
		ret = pthread_attr_getstacksize(&attr, &stacksize);
		if (ret) { 
			perror("pthread_attr_getstacksize err"); 
			exit(1);
		} else {
			static bool once = true;
			if (once) {
				std::cout << "default thread stacksize: " << stacksize << std::endl;
				once = false;
			}

		}

		// thread stacksize
		stacksize = 16384;// 16KB
		ret = pthread_attr_setstacksize(&attr, stacksize);
		if (ret) { 
			perror("pthread_attr_setstacksize err"); 
			exit(1);
		} else {
			static bool once = true;
			if (once) {
				std::cout << "change thread stacksize to : " << stacksize << std::endl;
				once = false;
			}
		}

		if (!(i % (MAX / 10000))) std::cerr << "..." << i;

		pthread_t thread;
		ret = pthread_create(&thread, &attr, thr_func, NULL);
		if (ret) {
			std::cerr << "\nthreads created " << i << std::endl ;
			perror("pthread_create");
			exit(1);
		}
	}

	std::cerr << "threads created " << i - 1 << std::endl ;
	std::cout << "exit (succes)" << std::endl;
}

int main()
{
	max_thread_test();
	for(;;) sleep(5000);

	std::cout << "end" << std::endl;
	return 0;
}
/* http://blog.csdn.net/delphiwcdj/article/details/53571982

$./test_max_thread 
default thread stacksize: 524288
change thread stacksize to : 16384
...0...419...838...1257...1676
threads created 2047
pthread_create: Cannot allocate memory

* */
