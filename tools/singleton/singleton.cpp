#include <iostream>
#include <pthread.h>
using namespace std;

class CSingleton 
{
public:
	static CSingleton& getInstance()
	{
		static CSingleton theOne;
		return theOne;
	}

	bool test(int i)
	{
		cout << "CSingleton:test(): " << i << endl;
		return true;
	}

private:
	CSingleton()
	{ 
		cout << "CSingleton construct beg" << endl;
		sleep(30); 
		cout << "CSingleton construct end" << endl;
	}
	~CSingleton()
	{
		cout << "CSingleton destruct" << endl;
	}
};

class CThreadNum
{
public:
	CThreadNum(){}
	~CThreadNum(){}

	void seti(int i)
	{
		m_i = i;
	}

public:
	int m_i;
};

void* ThreadHandle(void* tp)
{
	CThreadNum* pt = (CThreadNum*)tp;

	cout << "thread: " << pt->m_i << " start" << endl;

	CSingleton::getInstance().test(pt->m_i);

	cout << "thread: " << pt->m_i << " end" << endl;

	return NULL;
}


int main(int argc, char* argv[])
{

	cout << "main beg" << endl;    

	const int num = 10;

	CThreadNum t[num];

	for (int i = 0; i < num; ++i)
	{
		t[i].seti(i);
	}

	for (int i = 0; i < num; ++i)
	{   
		pthread_t id;
		pthread_create(&id, NULL, &ThreadHandle, &t[i]);
	}

	sleep(60);

	cout << "main end" << endl;    

	return 0;
}
/*
 http://stackoverflow.com/questions/16734966/static-local-variable-initialisation-in-multithreaded-environment

 g++ -o xxx singleton.cpp -lpthread

output:
main beg
thread: thread: thread: thread: 1 start
0 start2
 start
CSingleton construct beg
thread: 5 start
thread: 6 start
thread: 7 start
thread: 8 start
thread: 9 start
3 start
thread: 4 start
CSingleton construct end
CSingleton:test(): 0
thread: 0 end
CSingleton:test(): 2
thread: 2 end
CSingleton:test(): 5
thread: 5 end
CSingleton:test(): 6
thread: 6 end
CSingleton:test(): 7
thread: 7 end
CSingleton:test(): 8
thread: 8 end
CSingleton:test(): 9
thread: 9 end
CSingleton:test(): 1
thread: 1 end
CSingleton:test(): 3
thread: 3 end
CSingleton:test(): 4
thread: 4 end
 */



