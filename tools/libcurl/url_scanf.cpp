#include <cstdio>
#include <cstring>
#include <string>
using namespace std;

#define COMM_MIN_LENTH 128

int getUrlCgi(const std::string &strUrl, std::string &strHead, std::string &strDomain, std::string &strContent)
{
	char szHead[COMM_MIN_LENTH]      =  {0};
	char szDomain[COMM_MIN_LENTH]    =  {0};
	char szContent[COMM_MIN_LENTH*4] =  {0};

	sscanf(strUrl.c_str(), "%15[^\n:]://%[^\n/]%512[^\n]", szHead, szDomain, szContent);

	strHead    =  string(szHead);
	strDomain  =  string(szDomain);
	strContent =  string(szContent);

	return 0;
}

int main(int argc, char **argv)
{

	string strUrl = "http://10.135.158.26:9001/cgi-bin/demo_provide.cgi";
	string strHead;
	string strDomain;
	string strContent;

	getUrlCgi(strUrl, strHead, strDomain, strContent);
	printf("strUrl[%s]\nstrHead[%s]\nstrDomain[%s]\nstrContent[%s]\n",
			strUrl.c_str(), strHead.c_str(), strDomain.c_str(), strContent.c_str());

	return 0;
}
/* g++ -Wall -pipe -Os -o url_scanf url_scanf.cpp
output:

strUrl[http://10.135.158.26:9001/cgi-bin/demo_provide.cgi]
strHead[http]
strDomain[10.135.158.26:9001]
strContent[/cgi-bin/demo_provide.cgi]
*/

