#ifndef _RSATOOLS_H
#define _RSATOOLS_H

#include <stdio.h>
#include <string>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/engine.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include "Base64.h"
using namespace std;
using namespace BASE64;

#define COMM_MAX_BUFLEN 2048

namespace RSATOOLS 
{

	void str2Hex(unsigned char* buf, int& len, string& hex);
	int hex2Str(std::string &src, std::string &dst);

	int calcRsaSign(const char * szData, string& strSign, const char * szSignType, string& strPrivateKeyPath, int& iResultCode, string& strResultInfo);
	int calcRsaSignFromStr(const char * szData, string& strSign, const char * szSignType, string& strPrivateKey, int& iResultCode, string& strResultInfo);

	int verifyRsaSign(const char * szData, const char * szSign, const char * szSignType, string& strPubKeyPath, int& iResultCode, string& strResultInfo);
	int verifyRsaSignFromStr(const char * szData, const char * szSign, const char * szSignType, string& strPubKey, int& iResultCode, string& strResultInfo);

	int rsaEncrypt(const char * szData, string& strPubKeyPath, char * szCipherData, int& iCipherDataLen, const char * szCipherType, int& iResultCode, string& strResultInfo);
	int rsaDecrypt(const char * szCipher, const char * szCipherType, string& strPrivateKeyPath, char * szClearData, int& iClearDataLen, int& iResultCode, string& strResultInfo);

}

#endif /* _RSATOOLS_H */
