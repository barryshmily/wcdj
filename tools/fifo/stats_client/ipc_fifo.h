#ifndef TDF_IPC_FIFO_H_
#define TDF_IPC_FIFO_H_

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
#include <string.h>

namespace tdf 
{
	namespace ipc
	{

		struct fifo_fail: public std::runtime_error
		{ 
			fifo_fail(const std::string& s) : std::runtime_error(s){}
		};

		struct fifo_delay: public std::runtime_error
		{ 
			fifo_delay(const std::string& s) : std::runtime_error(s){}
		};


		class fifo 
		{
		public:
			fifo();
			~fifo();

		public:
			enum ENUM_RW_SET
			{
				EM_WRITE = 1,
				EM_READ  = 2,
				EM_RW    = 3
			};

		public:
			/* 
			 * open fifo
			 * @param pathname: fifo path
			 * @param rw_mode: read or write
			 * @throw fifo_fail: mkfifo error 
			 * */
			void open(const char *pathname, ENUM_RW_SET rw_mode, mode_t mode = 0664) throw(fifo_fail);

			/*
			 * close fifo
			 * */
			void close();

			int fd() {
				return fd_;
			}
			/* 
			 * read data
			 * @param buffer
			 * @param max_size: the size of buffer
			 * @return: the length of read
			 * @throw fifo_fail: fifo error
			 * @throw fifo_delay: EAGAIN
			 * */
			int read(char *buffer, size_t max_size) throw(fifo_fail, fifo_delay);
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
			std::string pathname_;
			int fd_;
			ENUM_RW_SET rw_mode_;
		};

	} /* ipc */
} /* tdf */

#endif /* TDF_IPC_FIFO_H_ */

