#include "ipc_fifo.h"
using namespace std;


// CFifo
wcdj::ipc::CFifo::CFifo()
{
	_fd = -1;
	_rorw = true;
}

wcdj::ipc::CFifo::~CFifo()
{
	close();
}

void wcdj::ipc::CFifo::close()
{
	if (_fd > 0) 
	{
		::close(_fd); 
		_fd = -1;
	}

	// Note: 一直到最后都没有删除这个FIFO文件，因为我们没办法知道是否有其他程序正在使用它
}

void wcdj::ipc::CFifo::open(const char *pathname, bool rorw, mode_t mode) throw(wcdj::ipc::fifo_fail)
{
	_rorw     =  rorw;
	_pathname =  pathname;

	if (mkfifo(pathname, mode)) 
	{
		if (errno == EEXIST) 
		{
			//cout << "fifo: " << pathname << " exist" << endl;
		} 
		else 
		{
			throw wcdj::ipc::fifo_fail(string("wcdj::ipc::CFifo mkfifo error: ") + pathname + " :" + strerror(errno));
		}
	}

	/* param rorw: read or write, true-read false-write
	 * Note: O_NONBLOCK分别搭配O_RDONLY和O_WRONLY在效果上的不同
	 * 如果没有进程以读方式打开管道，非阻塞写方式的open调用将失败
	 * 但非阻塞读方式的open调用总是成功
	 * close调用的行为并不受O_NONBLOCK标志的影响
	 * */
	if (rorw) 
	{
		//_fd = ::open(pathname, O_NONBLOCK | O_RDONLY, 0664);
		_fd = ::open(pathname, O_RDONLY, 0664);
		if (_fd < 0) 
		{
			throw wcdj::ipc::fifo_fail(string("wcdj::ipc::CFifo open read fifo fail:") + strerror(errno));
		}
	} 
	else 
	{
		_fd = ::open(pathname, O_NONBLOCK | O_WRONLY, 0664);
		if (_fd < 0) 
		{
			if (errno == ENXIO) 
			{
				//cout << "open fifo for write but no reading" << endl;
				return;
			}
			throw wcdj::ipc::fifo_fail(string("wcdj::ipc::CFifo open write fifo fail:") + strerror(errno));
		}
	}
}

int wcdj::ipc::CFifo::read(char * buffer, size_t max_size) throw(wcdj::ipc::fifo_fail, wcdj::ipc::fifo_delay)
{

	int iRetVal = 0;
	for (;;) 
	{
		iRetVal = ::read(_fd, buffer, max_size);
		if (iRetVal < 0) 
		{
			if (errno == EINTR) 
			{
				//cerr << "CFifo:read EINTR" << endl;
				continue;
			}
			else if (errno == EAGAIN) 
			{
				//cerr << "CFifo:read EAGAIN" << endl;
				throw wcdj::ipc::fifo_delay("wcdj::ipc::CFifo read EAGAIN");
			}
			else 
			{
				throw wcdj::ipc::fifo_fail(string("wcdj::ipc::CFifo read fifo fail:") + strerror(errno));
			}
		} 
		else if (iRetVal == 0)
		{
			//throw wcdj::ipc::fifo_fail("wcdj::ipc::CFifo read fifo closed by peer");

			// 阻塞读不能退出
		} 
		else 
		{
			/* 阻塞读一次能够读完吗? 若client不close fifo且读取的数据量大于一次read的buffer则需要对read函数进行重复调用, 
			 * 此时需要协议头部有个长度字段以判断是否接收完毕
			 *
			 * read调用：对一个空的，阻塞的FIFO的read调用将等待，直到有数据可以读时才继续执行；
			 * 与此相反，对一个空的非阻塞的FIFO的read调用将立刻返回0字节。
			 * */
			break;
		}
	}

	return iRetVal;
}

int wcdj::ipc::CFifo::write(const char *buffer, size_t buflen) throw(wcdj::ipc::fifo_fail, fifo_delay)
{
	if (buflen == 0) 
		return 0;

	// param rorw: read or write, true-read false-write
	if (_fd < 0 && (!_rorw)) 
	{
		// reopen fifo
		_fd = ::open(_pathname.c_str(), O_NONBLOCK | O_WRONLY, 0664);
		if (_fd < 0) 
		{
			// ENXIO  O_NONBLOCK | O_WRONLY is set, the named file is a FIFO and no process has the file open for reading. Or, the file is a device special file and no corresponding device exists.
			if (errno == ENXIO) 
			{
				throw fifo_delay(string("wcdj::ipc::CFifo write open fifo fail:") + strerror(errno));
			}
			throw wcdj::ipc::fifo_fail(string("wcdj::ipc::CFifo write open fifo fail:") + strerror(errno));
		}
	}

	int iRetVal = 0;
	for (;;) 
	{
		// buflen should be <= PIPE_BUF, otherwise it is not atom oper
		iRetVal = ::write(_fd, buffer, buflen);
		if (iRetVal < 0) 
		{
			if (errno == EINTR) 
			{
				//cerr << "CFifo:read EINTR" << endl;
				continue;
			}
			else if (errno == EAGAIN)
			{
				//cerr << "CFifo:write EAGAIN" << endl;
				throw fifo_delay(string("wcdj::ipc::CFifo write EAGAIN:") + strerror(errno));
			}
			else if (errno == EPIPE)  
			{
				throw wcdj::ipc::fifo_fail(string("wcdj::ipc::CFifo write EPIPE:") + strerror(errno));
			}
			else
			{
				throw wcdj::ipc::fifo_fail(string("wcdj::ipc::CFifo write fifo fail:") + strerror(errno));
			}
		} 
		else if (iRetVal == 0) 
		{
			throw wcdj::ipc::fifo_fail("wcdj::ipc::CFifo fifo closed by peer");
		} 
		else 
		{
			/* 异步写一次能够写完吗?
			 * write调用：对一个完全阻塞的FIFO的write调用将等待，直到数据可以被写入时才继续执行；
			 * 与此相反，非阻塞时，如果FIFO不能接收所有写入的数据，它将按下面的规则执行：
			 * [1] 如果请求写入的数据的长度<=PIPE_BUF大小，若调用失败，数据不能写入(即，系统保证原子操作)；
			 * [2] 如果请求写入的数据的长度>PIPE_BUF大小，将写入部分数据，返回实际写入的字节数，返回值也可能是0。
			 * */
			break;
		}
	}

	return iRetVal;
}

