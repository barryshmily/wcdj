#ifndef __WCDJ_IPC_FIFO_H__
#define __WCDJ_IPC_FIFO_H__

/**
 * progcess, thread, mutex related 
 */

#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/file.h>
#include <stdexcept>

namespace wcdj
{
	namespace ipc
	{

		struct fifo_fail: public std::runtime_error
		{ 
			fifo_fail(const std::string& s):std::runtime_error(s){}
		};

		struct fifo_delay: public std::runtime_error
		{ 
			fifo_delay(const std::string& s):std::runtime_error(s){}
		};


		class CFifo 
		{
		public:
			CFifo();
			~CFifo();
		public:
			/* 
			 * open fifo
			 * @param pathname: fifo path
			 * @param rorw: read or write, true-read false-write
			 * @throw fifo_fail: mkfifo error 
			 * */
			void open(const char *pathname, bool rorw, mode_t mode = 0777) throw(fifo_fail);
			/*
			 * close fifo
			 * */
			void close();
			int fd() {
				return _fd;
			}
			/* 
			 * read data
			 * @param buffer
			 * @param max_size: the size of buffer
			 * @return: the length of read
			 * @throw fifo_fail: fifo error
			 * @throw fifo_delay: EAGAIN
			 * */
			int read(char * buffer, size_t max_size) throw(fifo_fail, fifo_delay);
			/*
			 * write data
			 * @param buffer
			 * @param buflen: the size of data to be written
			 * @return: the length of written
			 * @throw fifo_fail: fifo error
			 * @throw fifo_delay: EAGAIN
			 * */
			int write(const char *buffer, size_t buflen) throw(fifo_fail, fifo_delay);

		private:
			std::string _pathname;
			int _fd;
			bool _rorw;
		};

	}
}

#endif /* __WCDJ_IPC_FIFO_H__ */

