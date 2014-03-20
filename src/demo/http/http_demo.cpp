#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include "http_handle.h"

using namespace std;

int main(int argc, char **argv)
{

#if 0
	string strIp    =  "172.25.81.16";
	string strPort  =  "80";
	string strURI   =  "/cgi-bin/test.cgi";
	string strParas =  "?a=b&c=d";
	string strReq   =  "http://" + strIp + ":" + strPort + strURI + strParas;
#endif

	string strReq = "http://172.25.81.16:80/cgi-bin/test.cgi?a=b&c=d";
	if (argc == 2)
	{
		strReq = argv[1];
	}
	printf("http req[%s]\n", strReq.c_str());

	int iTimeout =  1;// 1s

	map<string, string> mapPost;
	string strResp;
	string strErrMsg;
	int iResult = 0;

	int iRet = wcdj::http::http_proc(strReq, iTimeout, mapPost, strResp, iResult, strErrMsg);
	if (iRet != 0)
	{
		fprintf(stderr, "http_proc error, iret[%d] errmsg[%s] resp[%s]\n", iRet, strErrMsg.c_str(), strResp.c_str());
	}
	else
	{
		// for example, websvr returns {"ret":0,"msg":"OK"}
		if (strResp.find("\"ret\":0") == string::npos)
		{
			fprintf(stderr, "http_proc ok, but resp is error[%s]\n", strResp.c_str());
		}
		else
		{
			printf("http_proc ok, resp is ok[%s]\n", strResp.c_str());
		}
	}

	return 0;
}
