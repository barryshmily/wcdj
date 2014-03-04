#include "comm.h"
#include "COption.h"
#include "CServer.h"
#include "CAppConfig.h"


using namespace std;

// use command to verify prog version, such as strings test_svr | grep -i "WCDJ"
// the result likes, WCDJ|VersionV1.0R010_2014/02/20 18:44:38 |gcc_4.1.2 20070115 (prerelease) (SUSE Linux)
#define VERSION "WCDJ|VersionV1.0R010_"MY_DATE" |gcc_"__VERSION__

const unsigned char SIG_SHOW =  1;
const unsigned char SIG_QUIT =  2;
unsigned char SIG_STAT       =  0;


void Usage(const string& progname)
{
    cout << "Welcome to use WCDJ-svr, the version is:\n" << VERSION << " \n"
        << "Usage: " << progname << " [OPTION]..." << "\n"
        << "       " << progname << " -config=configfile [OPTION]..." << "\n"
        << "       OPTION format: -name=value" << "\n" << endl;
}

void sigusr1_handle(int iSigVal)
{
    SIG_STAT = SIG_SHOW;
    signal(SIGUSR1, sigusr1_handle);
}

void sigusr2_handle(int iSigVal)
{
    SIG_STAT = SIG_QUIT;
    signal(SIGUSR2, sigusr2_handle);
}

void Daemon()
{
	pid_t pid;

	if ((pid = fork()) != 0) 
	{
		exit(0); 
	}

	setsid();
	signal(SIGINT,    SIG_IGN);
	signal(SIGHUP,    SIG_IGN);
	signal(SIGQUIT,   SIG_IGN);
	signal(SIGPIPE,   SIG_IGN);
	signal(SIGTTOU,   SIG_IGN);
	signal(SIGTTIN,   SIG_IGN);
	signal(SIGCHLD,   SIG_IGN);
	signal(SIGTERM,   SIG_IGN);

	// ignore_pipe();
	struct sigaction sig;
	sig.sa_handler =  SIG_IGN;
	sig.sa_flags   =  0;
	sigemptyset(&sig.sa_mask);
	sigaction(SIGPIPE, &sig, NULL);

	if ((pid = fork()) != 0) 
	{ 
		exit(0);
	}

	//chdir("/");

	umask(0);

#ifdef NOPRINT_TERMINAL
	for (int i = 0; i < 64; ++i) 
		close(i);
#else
	for (int i = 3; i < 64; ++i) 
		close(i);
#endif

}

int CreatePIDFile(const char *sPIDFile)
{    
	FILE *pstFile;

	pstFile = fopen(sPIDFile, "w");
	if (pstFile == NULL)
	{
		fprintf(stderr, "Failed to open pid file:%s!\n", sPIDFile);
		return E_FAIL;
	}
	else
	{  
		fprintf(pstFile, "%d", getpid());
		fclose(pstFile);
	}

	return 0;
}

/*
 * prog entry
 * */
int main(int argc, char **argv)
{


    if (argc < 2)
    {
        Usage(argv[0]);
        return E_FAIL;
    } 

	CAppConfig& appconf_instance = CAppConfig::getapp_config_instance();

	COption opt;
	opt.read_arg(argc, argv);

	// if find config file, then read firstly
	if (opt["config"] != "")
	{
		// TODO
	}

	// after reading config from file then read terminal para
	
	if (opt["projecthome"] != "")
	{
		appconf_instance.set_projecthome(opt["projecthome"].c_str());
	}
	if (opt["threadcnt"] != "")
	{
		appconf_instance.set_threadcnt((unsigned)atoi(opt["threadcnt"].c_str()));
	}
	if (opt["processcnt"] != "")
	{
		appconf_instance.set_processcnt((unsigned)atoi(opt["processcnt"].c_str()));
	}
	if (opt["requestcnt"] != "")
	{
		appconf_instance.set_requestcnt((unsigned)atoi(opt["requestcnt"].c_str()));
	}

	appconf_instance.check_conf();
	
	Daemon();

	// prevent multi instance
	string strHome    =  opt["projecthome"];
	string strProg    =  argv[0];
	string strPidFile =  strHome + "/bin/" + strProg + ".pid";

	if (CreatePIDFile(strPidFile.c_str()) != 0)
	{
		cerr << "CreatePIDFile err, so quit!" << endl;
		return E_FAIL;
	}

	try 
	{
		signal(SIGUSR1, sigusr1_handle);
		signal(SIGUSR2, sigusr2_handle);

		CServer* server = new CServer();
		server->init(argc, argv);
		server->run();
	}
	catch (runtime_error& e)
	{
		cerr << "catch runtime_error:" << e.what() << "\n" << endl;
		return E_FAIL;
	}
	catch (logic_error& e)
	{
		cerr << "catch logic_error:" << e.what() << "\n" << endl;
		return E_FAIL;
	}

	return 0;
}
