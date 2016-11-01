#include "ipc_fifo.h"

tdf::ipc::fifo::fifo() : fd_(-1), rw_mode_(EM_WRITE)
{
}

tdf::ipc::fifo::~fifo()
{
	close();
}

void tdf::ipc::fifo::close()
{
	if (fd_ >= 0) 
	{
		::close(fd_); 
		fd_ = -1;
	}

	// note, we do not delete fifo file here
}

void tdf::ipc::fifo::open(const char *pathname, ENUM_RW_SET rw_mode, mode_t mode) throw(tdf::ipc::fifo_fail)
{
	rw_mode_  =  rw_mode;
	pathname_ =  pathname;

	// so we get the permissions we want 
	umask(0); 

	if (mkfifo(pathname, mode)) 
	{
		if (errno == EEXIST) 
		{
			// fifo exist
		} 
		else 
		{
			throw tdf::ipc::fifo_fail(std::string("tdf::ipc::fifo mkfifo error: ") + pathname + " :" + strerror(errno));
		}
	}

	/* 
	 * A process can open a FIFO in nonblocking mode. 
	 * In this case, opening for read only will succeed even if no-one has opened on the write side yet, 
	 * opening for write only will fail with ENXIO (no such device or address) unless the other end has already been opened.
	 * Under Linux, opening a FIFO for read and write will succeed both in blocking and nonblocking mode. 
	 * POSIX leaves this behavior undefined. This can be used to open a FIFO for writing while there are no readers available.
	 */
	if (rw_mode_ == EM_READ) 
	{
		fd_ = ::open(pathname, O_NONBLOCK | O_RDONLY, 0664);
		if (fd_ < 0) 
		{
			throw tdf::ipc::fifo_fail(std::string("tdf::ipc::fifo open read fifo fail:") + strerror(errno));
		}
	} 
	else 
	{
		fd_ = ::open(pathname, O_NONBLOCK | O_WRONLY, 0664);
		if (fd_ < 0) 
		{
			if (errno == ENXIO) 
			{
				// open fifo for write but no reading

				// TODO 告警, 读进程已经不存在会影响数据统计
				return;
			}
			throw tdf::ipc::fifo_fail(std::string("tdf::ipc::fifo open write fifo fail:") + strerror(errno));
		}
	}

	return;
}

int tdf::ipc::fifo::read(char *buffer, size_t max_size) throw(tdf::ipc::fifo_fail, tdf::ipc::fifo_delay)
{
	int ret = 0;

	for (;;) 
	{
		ret = ::read(fd_, buffer, max_size);
		if (ret < 0) 
		{
			if (errno == EINTR) 
			{
				continue;
			}
			else if (errno == EAGAIN) 
			{
				throw tdf::ipc::fifo_delay("tdf::ipc::fifo read EAGAIN");
			}
			else 
			{
				throw tdf::ipc::fifo_fail(std::string("tdf::ipc::fifo read fifo fail:") + strerror(errno));
			}
		} 
		else if (ret == 0)
		{
			// read fifo closed by peer

			// TODO
			// 若所有写进程都退出了, 非阻塞读进程会遇到结束标记0
			break;
		} 
		else 
		{
			// normal
			break;
		}
	}

	return ret;
}

int tdf::ipc::fifo::write(const char *buffer, size_t buflen) throw(tdf::ipc::fifo_fail, fifo_delay)
{
	if (buflen == 0)
	{
		return 0;
	}

	if (fd_ < 0 && rw_mode_ == EM_WRITE) 
	{
		// reopen fifo
		fd_ = ::open(pathname_.c_str(), O_NONBLOCK | O_WRONLY);
		if (fd_ < 0) 
		{
			// ENXIO  O_NONBLOCK | O_WRONLY is set, the named file is a FIFO and no process has the file open for reading. 
			// Or, the file is a device special file and no corresponding device exists.
			if (errno == ENXIO) 
			{
				throw fifo_delay(std::string("tdf::ipc::fifo write open fifo fail:") + strerror(errno));
			}
			throw tdf::ipc::fifo_fail(std::string("tdf::ipc::fifo write open fifo fail:") + strerror(errno));
		}
	}

	int ret = 0;
	for (;;) 
	{
		// buflen should be <= PIPE_BUF, otherwise it is not atom oper
		ret = ::write(fd_, buffer, buflen);
		if (ret < 0) 
		{
			if (errno == EINTR) 
			{
				continue;
			}
			else if (errno == EAGAIN)
			{
				throw fifo_delay(std::string("tdf::ipc::fifo write EAGAIN:") + strerror(errno));
			}
			else if (errno == EPIPE)  
			{
				throw tdf::ipc::fifo_fail(std::string("tdf::ipc::fifo write EPIPE:") + strerror(errno));
			}
			else
			{
				throw tdf::ipc::fifo_fail(std::string("tdf::ipc::fifo write fifo fail:") + strerror(errno));
			}
		} 
		else if (ret == 0) 
		{
			throw tdf::ipc::fifo_fail("tdf::ipc::fifo fifo closed by peer");
		} 
		else 
		{
			// normal
			break;
		}
	}

	return ret;
}

