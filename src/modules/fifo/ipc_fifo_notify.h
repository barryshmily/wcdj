
#ifndef __WCDJ_IPC_FIFO_NOTIFY_H__
#define __WCDJ_IPC_FIFO_NOTIFY_H__

#include <pthread.h>
#include "ipc_fifo.h"

using wcdj::ipc::fifo_delay;
using wcdj::ipc::fifo_fail;
using wcdj::ipc::CFifo;


/*
 * CNotify里的加锁操作貌似没有意义, 暂时保留
 * */

class CNotify: public CFifo 
{
public:
	CNotify():_mutex(NULL){}
	void mutex(pthread_mutex_t *mutex) 
	{ 
		_mutex = mutex;
	}

	void set_notify() throw(fifo_fail, fifo_delay)
	{
		char buffer[] = "Hello from client"; 

		if (_mutex) pthread_mutex_lock(_mutex);

		try 
		{
			// buflen should be <= PIPE_BUF, otherwise it is not atom oper
			write(buffer, strlen(buffer) + 1);
		} 
		catch (fifo_delay& e) 
		{
			if (_mutex) pthread_mutex_unlock(_mutex);
			throw e;
		} 
		catch (fifo_fail& e) 
		{
			if (_mutex) pthread_mutex_unlock(_mutex);
			throw e;
		}

		if (_mutex) pthread_mutex_unlock(_mutex);
	}

	void get_notify(char *buffer, unsigned bytes) throw(fifo_fail, fifo_delay)
	{

		if (_mutex) pthread_mutex_lock(_mutex);

		try 
		{
			read(buffer, bytes);
		} 
		catch (fifo_delay& e) 
		{
			if (_mutex) pthread_mutex_unlock(_mutex);
			throw e;
		}
		catch (fifo_fail& e) 
		{
			if (_mutex) pthread_mutex_unlock(_mutex);
			throw e;
		}

		if (_mutex) pthread_mutex_unlock(_mutex);
	}

private:
	pthread_mutex_t *_mutex;

};

#endif /* __WCDJ_IPC_FIFO_NOTIFY_H__ */

