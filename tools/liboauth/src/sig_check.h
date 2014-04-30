/* 
 * sig_check.h
 * 统一的签名验证类
 * oauth: http://liboauth.sourceforge.net/index.html
*/

#ifndef _SIG_CHECK_H_
#define _SIG_CHECK_H_

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

#ifdef __cplusplus
extern "C"{
#endif
#include "oauth.h"
#ifdef __cplusplus
};
#endif

using namespace std;

#ifndef SIGCHECK_ERROR
#define SIGCHECK_ERROR(fmt, args...) \
	snprintf(m_szErrMsg, sizeof(m_szErrMsg), "[%s][%d][%s]"fmt, \
			__FILE__, __LINE__, __FUNCTION__, ##args)
#endif


namespace NS_SIG_CHECK
{

	struct StSigInfo
	{
		string method;               // GET or POST
		string url_path;
		map<string, string> params;

		string share_secret;

		string desc()
		{
			stringstream ss;
			ss <<  "share_secret:" <<  share_secret <<  ",";
			ss <<  "method:"       <<  method       <<  ",";
			ss <<  "url_path:"     <<  url_path     <<  ",";

			ss << "params:";

			for (map<string, string>::iterator it = params.begin(); it != params.end(); ++it)
			{
				ss << "\t" << it->first << ":" << it->second;
			}

			return ss.str();
		}
	};

	class CSigCheck
	{
	public:
		CSigCheck() {}
		virtual ~CSigCheck() {}

		const char* GetErrMsg() { return m_szErrMsg; }

		string Create(StSigInfo& data)
		{
			string source;

			transform(data.method.begin(), data.method.end(), data.method.begin(), ::toupper);

			source.append(data.method);
			source.append("&");
			source.append(url_encode(data.url_path));
			source.append("&");
			source.append(url_encode(join_params(data.params)));

			char* p_sig = oauth_sign_hmac_sha1_raw(
					source.c_str(),
					source.size(), 
					data.share_secret.c_str(),
					data.share_secret.size());

			if (p_sig == NULL)
			{
				return "";
			}

			string sig = p_sig;;

			delete [] p_sig;
			p_sig = NULL;

			return sig;
		}

		int Check(StSigInfo& data, const string& param_sig)
		{
			if (param_sig.empty())
			{
				SIGCHECK_ERROR("param_sig is empty");
				return -1;
			}

			string real_sig = Create(data);

			if (real_sig != param_sig)
			{
				SIGCHECK_ERROR("sig not match, param_sig[%s], real_sig[%s], sig_info[%s]", 
						param_sig.c_str(), 
						real_sig.c_str(),
						data.desc().c_str()
						);
				return -2;
			}

			return 0;
		}

		string url_encode(const string& src)
		{
			char* p_dest = oauth_url_escape(src.c_str());
			if (p_dest == NULL)
			{
				return "";
			}
			string str_dest = p_dest;

			delete [] p_dest;
			p_dest = NULL;

			return str_dest;
		}

	private:

		string join_params(map<string, string> &params)
		{
			string source;
			for (map<string, string>::iterator it = params.begin(); it != params.end(); ++it)
			{
				if (it != params.begin())
				{
					source.append("&");
				}
				source.append(it->first);
				source.append("=");
				source.append(it->second);
			}

			return source;
		}

	private:
		char m_szErrMsg[512];
	};

} 

#endif /* _SIG_CHECK_H_ */
