#ifndef __WCDJ_SERVER_H__
#define __WCDJ_SERVER_H__

#include <unistd.h>
#include <stdexcept>
using namespace std;

extern const unsigned char SIG_SHOW;
extern const unsigned char SIG_QUIT;
extern unsigned char SIG_STAT;


class CServer 
{
public:
	CServer();
	CServer(int ival);
	~CServer();

	void init(int argc, char** argv) throw(runtime_error, logic_error);
	void run();
	int getVal();

private:
	int m_ival;
};

#endif
