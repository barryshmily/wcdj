#include "baseutils.h"
#include <openssl/des.h>
namespace baseutils
{
	char toHexChar(unsigned char d)
	{
		if(d <= 9)
			return '0' + d;
		return 'A' + d - 10;
	}
	std::string toHex(const std::vector<char> &vecData)
	{
		return toHex(vecData.data(),vecData.size());
	}
	std::string toHex(const std::vector<unsigned char> &vecData)
	{
		return toHex(vecData.data(),vecData.size());
	}
	std::string toHex(const void *pszData,int nLen)
	{	
		const unsigned char *p = (const unsigned char *)pszData;
		std::string strResult;
		strResult.reserve(nLen*2);
		for(int i=0;i<nLen;i++)
		{
			strResult += toHexChar((p[i] >> 4) & 0xF);
			strResult += toHexChar(p[i] & 0xF);
		}
		return strResult;
	}
	bool isHexChar(char ch)
	{
		if(ch >= '0' && ch <= '9')
			return true;
		if(ch >= 'a' && ch <= 'f')
			return true;
		if(ch>='A' && ch <= 'Z')
			return true;
		return false;
	}
	unsigned char fromHexChar(char ch)
	{
		if(ch >= '0' && ch <= '9')
			return ch-'0';
		if(ch >= 'a' && ch <= 'f')
			return ch-'a'+10;
		if(ch>='A' && ch <= 'Z')
			return ch - 'A'+10;
		return 0;
	}
	bool fromHex(const std::string &strSource,std::vector<unsigned char> &vecData)
	{
		vecData.clear();
		vecData.reserve((strSource.length()+1)/2);
		size_t nPos = 0;
		size_t nLen = strSource.length();
		char ch = 0;
		if(nLen % 2 > 0)
		{
			ch = fromHexChar(strSource[0]);
			vecData.push_back(ch);
			nPos = 1;
		}
		for(size_t i = nPos;i < nLen;i+=2)
		{
			if(!isHexChar(strSource[i]) || !isHexChar(strSource[i+1]))
				vecData.push_back('?');
			ch = (fromHexChar(strSource[i])<<4) | fromHexChar(strSource[i+1]);
			vecData.push_back(ch);
		}
		return true;
	}
	std::string des_pkcs_ecb_enc(const std::string &strData,const std::string &strKey)
	{
		//init cached vector
		int nLength = (strData.length()+8)>>3<<3;
		std::vector<unsigned char> vecTemp(nLength,0);
		//init key
		DES_cblock ckey = {0};
		if(strKey.length() < 8)
		{
			memcpy(&ckey,strKey.data(),strKey.length());
		}
		else
		{
			memcpy(&ckey,strKey.data(),8);
		}
		DES_key_schedule schedule;
		DES_set_key_unchecked(&ckey, &schedule);
		//encrypt
		for(int i=0;i<nLength;i+=8)
		{
			if(i + 8 < strData.length())
			{
				DES_ecb_encrypt((const_DES_cblock *)(strData.data()+i), (DES_cblock*)(vecTemp.data()+i), &schedule, DES_ENCRYPT);
			}
			else
			{
				DES_cblock source = {0};
				int n = strData.length()-i;
				memcpy(source,strData.data()+i,n);
				memset(source+n,8-n,8-n);
				DES_ecb_encrypt((const_DES_cblock *)source, (DES_cblock*)(vecTemp.data()+i), &schedule, DES_ENCRYPT);
			}
		}
		//convert hex string
		return toHex(vecTemp);
	}
	std::string des_pkcs_ecb_dec(const std::string &strData,const std::string &strKey)
	{
		//convert data
		std::vector<unsigned char> vecSource;
		if(!fromHex(strData,vecSource))
		{
			//throw exception or return empty string
			return "";
		}
		if(vecSource.size() % 8 > 0)
		{
			//throw exception or return empty string
			return "";
		}
		//init key
		DES_cblock ckey = {0};
		if(strKey.length() < 8)
		{
			memcpy(&ckey,strKey.data(),strKey.length());
		}
		else
		{
			memcpy(&ckey,strKey.data(),8);
		}
		DES_key_schedule schedule;
		DES_set_key_unchecked(&ckey, &schedule);
		//decrypt data
		std::vector<unsigned char> vecDest(vecSource.size(),0);
		for(int i = 0;i<vecSource.size();i += 8)
		{
			DES_ecb_encrypt((const_DES_cblock *)(vecSource.data()+i), (DES_cblock*)(vecDest.data()+i), &schedule, DES_DECRYPT);
		}
		//compute output length
		if(vecDest.size() <= 0)
			return "";
		int nLen = vecDest.size() - vecDest[vecDest.size()-1];
		if(nLen < 0 || nLen >= vecDest.size())
		{
			//throw exception or return empty string
			return "";
		}
		std::string strRet;
		strRet.reserve(nLen);
		strRet.append(vecDest.begin(),vecDest.begin()+nLen);
		return strRet;
	}
}