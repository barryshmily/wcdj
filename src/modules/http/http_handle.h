#ifndef __WCDJ_HTTP_HANDLE_H__
#define __WCDJ_HTTP_HANDLE_H__

#include "curl/curl.h"
#include <string>
#include <map>

namespace wcdj 
{
	namespace http
	{
		size_t recv_proc(void *ptr, size_t size, size_t nmemb, std::string *p);

		int http_proc(const std::string &strUrl, 
				unsigned iTimeout, 
				std::map<std::string, std::string> &mapPostData, 
				std::string &strRecvBuf,
				int &iResult,
				std::string &strErrmsg,
				const std::string &strCookie = "");
	}

}

#endif /* __WCDJ_HTTP_HANDLE_H__ */
