#ifndef _BASEUTILS_H_
#define _BASEUTILS_H_
#include <string>
#include <vector>
//对中文的支持最好先进行编解码
namespace baseutils
{
	char toHexChar(unsigned char d);
	std::string toHex(const std::vector<char> &vecData);
	std::string toHex(const std::vector<unsigned char> &vecData);
	std::string toHex(const void *pszData,int nLen);
	bool isHexChar(char ch);
	unsigned char fromHexChar(char ch);
	bool fromHex(const std::string &strSource,std::vector<unsigned char> &vecData);

	std::string des_pkcs_ecb_enc(const std::string &strData,const std::string &strKey);
	std::string des_pkcs_ecb_dec(const std::string &strData,const std::string &strKey);
}
#endif