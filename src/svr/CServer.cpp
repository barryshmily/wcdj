#include "CServer.h"
#include "comm.h"

CServer::CServer()
{
}

CServer::CServer(int ival)
{
	m_ival = ival;
}

CServer::~CServer()
{
}

void CServer::init(int argc, char** argv) throw(runtime_error, logic_error)
{
	INFO("init ok!\n");
	return;
}

void CServer::run()
{
	for (;;)
	{
		try
		{
			if (SIG_STAT == SIG_SHOW)
			{
				// TODO
				INFO("get SIG_SHOW sig\n");
				SIG_STAT = 0;
			}
			else if (SIG_STAT == SIG_QUIT)
			{
				INFO("get SIG_QUIT sig, exit normal\n");
				exit(0);
			}

			// TODO
			INFO("running...\n");
			__LOG("running...\n");

			sleep(1);
		}
		catch (std::bad_alloc &e)
		{
			ERROR("catch bad_alloc exception\n");
		}
		catch (...)
		{
			ERROR("catch unkown exception\n");
		}
	}
}

int CServer::getVal()
{
	return m_ival;
}


