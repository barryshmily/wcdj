#include "hmac_tools.h"
#include "Base64.h"

using namespace BASE64;

std::string hmac_sha1_base64(std::string &key, std::string &data)
{

	unsigned char* digest;

	// You may use other hash engines. e.g EVP_md5(), EVP_sha224, EVP_sha512, etc
	digest = HMAC(EVP_sha1(), (unsigned char *)key.c_str(), key.length(), (unsigned char *)data.c_str(), data.length(), NULL, NULL);    
	
	// Be careful of the length of string with the choosen hash engine. SHA1 produces a 20-byte hash value which rendered as 40 characters.
	// Change the length accordingly with your choosen hash engine

	int iSignBase64                  =  2048;
	unsigned char szSignBase64[2048] =  {0};
	int iSign                        =  20;

	iSignBase64 =  EncodeBase64((unsigned char *)digest, szSignBase64, iSignBase64, iSign);

	return (char *)szSignBase64;
}

std::string hmac_sha1_hex(std::string &key, std::string &data)
{

	unsigned char* digest;

	// You may use other hash engines. e.g EVP_md5(), EVP_sha224, EVP_sha512, etc
	digest = HMAC(EVP_sha1(), (unsigned char *)key.c_str(), key.length(), (unsigned char *)data.c_str(), data.length(), NULL, NULL);    
	
	// Be careful of the length of string with the choosen hash engine. SHA1 produces a 20-byte hash value which rendered as 40 characters.
	// Change the length accordingly with your choosen hash engine

	char szHexHMACSha1[41];
	for (int i = 0; i < 20; ++i) {
		sprintf(&szHexHMACSha1[i * 2], "%02x", (unsigned int)digest[i]);
	}
	szHexHMACSha1[40] = '\0';

	return szHexHMACSha1;
}
