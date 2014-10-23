#include <stdio.h>
#include <string>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/engine.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include "Base64.h"
using namespace std;
using namespace WCDJ;

#define COMM_MAX_BUFLEN 2048


void str2Hex(const char * name, unsigned char* buf, int& len, string& hex)
{
	for (int i = 0; i < len; ++i)
	{
		char szTmp[3] = {0};
		snprintf(szTmp, sizeof(szTmp), "%02X", buf[i]);
		hex += szTmp;
	}
	printf("%s Hex [%d:%s]\n", name, len, hex.c_str());
}

int hextoStr(std::string &src, std::string &dst)
{
    dst = "";
    for (size_t i = 0; i < src.size(); i += 2)
    {
        int c = 0;
        sscanf(src.c_str() + i, "%2X", &c);
        dst += (char)c;   
    }

    return 0;
}

// use private.key to calc sign
int calcRsaSign(const char * szData, string& strSign, string& strPrivateKeyPath, int& iResultCode, string& strResultInfo)
{
	int iRet = 0;

	FILE *file     =  NULL;
	RSA *rsa       =  NULL;
	EVP_PKEY *pKey =  NULL;

	if ((file = fopen(strPrivateKeyPath.c_str(), "r")) == NULL)
	{
		iResultCode   =  -1;
		strResultInfo =  "fopen err:" + strPrivateKeyPath;

		return -1;
	}

	// should include <openssl/pem.h>
	// The RSAPrivateKey functions process an RSA private key using an RSA structure. 
	// It handles the same formats as the PrivateKey functions but an error occurs if the private key is not RSA.
	if ((rsa = PEM_read_RSAPrivateKey(file, NULL, NULL, NULL)) == NULL)
	{
		iResultCode   =  -2;
		strResultInfo =  "PEM_read_RSA_PUBKEY err";

		fclose(file);
		return -1;
	}

	// should include <openssl/evp.h>
	// private key allocation, EVP_PKEY_free() frees up the private key
	pKey = EVP_PKEY_new();
	if (pKey == NULL)
	{
		iResultCode   =  -3;
		strResultInfo =  "EVP_PKEY_new err";

		RSA_free(rsa);
		fclose(file);
		return -1;
	}

	// should include <openssl/evp.h>
	// set the key referenced by pKey to rsa
	// return 1 for success or 0 for failure
	if (EVP_PKEY_set1_RSA(pKey, rsa) != 1)
	{
		iResultCode   =  -4;
		strResultInfo =  "EVP_PKEY_set1_RSA err";

		RSA_free(rsa);
		EVP_PKEY_free(pKey);
		fclose(file);
		return -1;
	}

	// calc digest
	unsigned char ucDigest[COMM_MAX_BUFLEN] =  {0};
	int iDigest                             =  sizeof(ucDigest);

	// should include <openssl/evp.h>
	// The EVP digest routines are a high level interface to message digests, 
	// and should be used instead of the cipher-specific functions
	EVP_MD_CTX ctx;
	// initializes digest context ctx
	EVP_MD_CTX_init(&ctx);
	EVP_DigestInit(&ctx, EVP_sha1());
	EVP_DigestUpdate(&ctx, szData, strlen(szData));
	EVP_DigestFinal(&ctx, ucDigest, (unsigned int *)&iDigest);

	// calc sign
	int iSign              =  RSA_size(rsa);
	printf("RSA_size(rsa)[%d]\n", iSign);
	unsigned char * ucSign =  (unsigned char *)malloc(sizeof(unsigned char) * iSign);
	memset(ucSign, 0x0, sizeof(unsigned char) * iSign);

	// should include <openssl/rsa.h>
	// RSA_sign() signs the message digest m of size m_len using the private key rsa as specified in PKCS #1 v2.0. 
	// It stores the signature in sigret and the signature size in siglen. sigret must point to RSA_size(rsa) bytes of memory. 
	// Note that PKCS #1 adds meta-data, placing limits on the size of the key that can be used. See RSA_private_encrypt(3) for lower-level operations.
	//iRet = RSA_sign(NID_sha1, (unsigned char *)szData, strlen(szData), ucSign, (unsigned int *)&iSign, rsa);
	iRet = RSA_sign(NID_sha1, ucDigest, iDigest, ucSign, (unsigned int *)&iSign, rsa);
	if (iRet != 1)
	{
		iResultCode   =  -5;

		char szErr[1024] = {0};
		ERR_error_string(ERR_get_error(), szErr);
		strResultInfo =  string("RSA_sign err: ") + szErr;

		free(ucSign);
		RSA_free(rsa);
		EVP_PKEY_free(pKey);
		fclose(file);
		EVP_MD_CTX_cleanup(&ctx);
		return -1;
	}

	// encodebase64 sign 
	unsigned char ucSignBase64[COMM_MAX_BUFLEN] =  {0};
	int iSignBase64                             =  sizeof(ucSignBase64);
	iSignBase64 = EncodeBase64((unsigned char *)ucSign, ucSignBase64, iSignBase64, iSign);
	printf("sign EncodeBase64 [%d:%s]\n", iSignBase64, ucSignBase64);

	// debug
	string strSignHex;
	str2Hex("sign", ucSign, iSign, strSignHex);
	strSign = strSignHex;

	free(ucSign);
	RSA_free(rsa);
	EVP_PKEY_free(pKey);
	fclose(file);
	EVP_MD_CTX_cleanup(&ctx);

	return 0;
}

// use public.key to verify sign
int verifyRsaSign(const char * szData, const char * szSign, const char * szSignType, string& strPubKeyPath, int& iResultCode, string& strResultInfo)
{ 
	int iRet = 0;

	FILE *file     =  NULL;
	RSA *rsa       =  NULL;
	EVP_PKEY *pKey =  NULL;

	if ((file = fopen(strPubKeyPath.c_str(), "r")) == NULL)
	{
		iResultCode   =  -1;
		strResultInfo =  "fopen err:" + strPubKeyPath;

		return -1;
	}

	// should include <openssl/pem.h>
	// The default public key file format generated by openssl is the PEM format
	// PEM_read_RSA_PUBKEY() reads the PEM format.
	if ((rsa = PEM_read_RSA_PUBKEY(file, NULL, NULL, NULL)) == NULL)
	{
		iResultCode   =  -2;
		strResultInfo =  "PEM_read_RSA_PUBKEY err";

		fclose(file);
		return -1;
	}

	// should include <openssl/evp.h>
	// private key allocation, EVP_PKEY_free() frees up the private key
	pKey = EVP_PKEY_new();
	if (pKey == NULL)
	{
		iResultCode   =  -3;
		strResultInfo =  "EVP_PKEY_new err";

		RSA_free(rsa);
		fclose(file);
		return -1;
	}

	// should include <openssl/evp.h>
	// set the key referenced by pKey to rsa
	// return 1 for success or 0 for failure
	if (EVP_PKEY_set1_RSA(pKey, rsa) != 1)
	{
		iResultCode   =  -4;
		strResultInfo =  "EVP_PKEY_set1_RSA err";

		RSA_free(rsa);
		EVP_PKEY_free(pKey);
		fclose(file);
		return -1;
	}

	// calc digest
	unsigned char ucDigest[COMM_MAX_BUFLEN] =  {0};
	int iDigest                             =  sizeof(ucDigest);

	// should include <openssl/evp.h>
	// The EVP digest routines are a high level interface to message digests, 
	// and should be used instead of the cipher-specific functions
	EVP_MD_CTX ctx;
	// initializes digest context ctx
	EVP_MD_CTX_init(&ctx);
	EVP_DigestInit(&ctx, EVP_sha1());
	EVP_DigestUpdate(&ctx, szData, strlen(szData));
	EVP_DigestFinal(&ctx, ucDigest, (unsigned int *)&iDigest);


	unsigned char ucSign[COMM_MAX_BUFLEN] =  {0};
	int iSign                             =  sizeof(ucSign);

	if (string(szSignType) == "base64")
	{
		// decodebase64 sign
		iSign = DecodeBase64((unsigned char *)szSign, ucSign, COMM_MAX_BUFLEN, strlen(szSign));
	}
	else if (string(szSignType) == "hex")
	{
		string strSignHex = szSign;
		string strSign;
		hextoStr(strSignHex, strSign);
		iSign = strSign.length();
		memcpy(ucSign, strSign.c_str(), iSign);
	}


	//------------------------------------------
	// debug
	string strSignHex;
	str2Hex("sign", ucSign, iSign, strSignHex);

	string strDigestHex;
	str2Hex("digest", ucDigest, iDigest, strDigestHex);
	//------------------------------------------

	// should include <openssl/rsa.h>
	// RSA_verify() verifies that the signature sigbuf of size siglen matches a given message digest m of size m_len. 
	// type denotes the message digest algorithm that was used to generate the signature. rsa is the signer's public key
	iRet = RSA_verify(NID_sha1, ucDigest, iDigest, ucSign, iSign, rsa);
	if (iRet != 1)
	{
		iResultCode   =  -5;
		strResultInfo =  "RSA_verify err";

		RSA_free(rsa);
		EVP_PKEY_free(pKey);
		fclose(file);
		EVP_MD_CTX_cleanup(&ctx);
		return -1;
	}

	RSA_free(rsa);
	EVP_PKEY_free(pKey);
	fclose(file);
	EVP_MD_CTX_cleanup(&ctx);

	return 0;
}

// rsa encrypt
int rsaEncrypt(const char * szData, string& strPubKeyPath, char * szCipherData, int& iCipherDataLen, int& iResultCode, string& strResultInfo)
{
	int iRet = 0;

	FILE *file     =  NULL;
	RSA *rsa       =  NULL;
	EVP_PKEY *pKey =  NULL;

	if ((file = fopen(strPubKeyPath.c_str(), "r")) == NULL)
	{
		iResultCode   =  -1;
		strResultInfo =  "fopen err:" + strPubKeyPath;

		return -1;
	}

	// should include <openssl/pem.h>
	// The default public key file format generated by openssl is the PEM format
	// PEM_read_RSA_PUBKEY() reads the PEM format.
	if ((rsa = PEM_read_RSA_PUBKEY(file, NULL, NULL, NULL)) == NULL)
	{
		iResultCode   =  -2;
		strResultInfo =  "PEM_read_RSA_PUBKEY err";

		fclose(file);
		return -1;
	}

	// should include <openssl/evp.h>
	// private key allocation, EVP_PKEY_free() frees up the private key
	pKey = EVP_PKEY_new();
	if (pKey == NULL)
	{
		iResultCode   =  -3;
		strResultInfo =  "EVP_PKEY_new err";

		RSA_free(rsa);
		fclose(file);
		return -1;
	}

	// should include <openssl/evp.h>
	// set the key referenced by pKey to rsa
	// return 1 for success or 0 for failure
	if (EVP_PKEY_set1_RSA(pKey, rsa) != 1)
	{
		iResultCode   =  -4;
		strResultInfo =  "EVP_PKEY_set1_RSA err";

		RSA_free(rsa);
		EVP_PKEY_free(pKey);
		fclose(file);
		return -1;
	}

	printf("before RSA_public_encrypt: szData len[%lu] szCipherData len[%d]\n", strlen(szData), iCipherDataLen);
	// RSA_PKCS1_OAEP_PADDING
	iCipherDataLen = RSA_public_encrypt(strlen(szData), (const unsigned char *)szData, (unsigned char *)szCipherData, rsa, RSA_PKCS1_OAEP_PADDING);
	// RSA_NO_PADDING
	////iCipherDataLen = RSA_public_encrypt(strlen(szData), (const unsigned char *)szData, (unsigned char *)szCipherData, rsa, RSA_NO_PADDING);
	printf("after RSA_public_encrypt: szData len[%lu] szCipherData len[%d]\n", strlen(szData), iCipherDataLen);

	//------------------------------------------
	// debug
	string strCipherDataHex;
	str2Hex("cipher", (unsigned char *)szCipherData, iCipherDataLen, strCipherDataHex);

	// encodebase64 cipher
	unsigned char ucCipherBase64[COMM_MAX_BUFLEN] =  {0};
	int iCipherBase64                             =  sizeof(ucCipherBase64);

	iCipherBase64 = EncodeBase64((unsigned char *)szCipherData, ucCipherBase64, iCipherBase64, iCipherDataLen);
	printf("cipher EncodeBase64 [%d:%s]\n", iCipherBase64, ucCipherBase64);

	//------------------------------------------

	RSA_free(rsa);
	EVP_PKEY_free(pKey);
	fclose(file);

	return 0;
}

// rsa decrypt
int rsaDecrypt(const char * szData, string& strPrivateKeyPath, char * szClearData, int& iClearDataLen, int& iResultCode, string& strResultInfo)
{
	int iRet = 0;

	FILE *file     =  NULL;
	RSA *rsa       =  NULL;
	EVP_PKEY *pKey =  NULL;

	if ((file = fopen(strPrivateKeyPath.c_str(), "r")) == NULL)
	{
		iResultCode   =  -1;
		strResultInfo =  "fopen err:" + strPrivateKeyPath;

		return -1;
	}

	// should include <openssl/pem.h>
	// The RSAPrivateKey functions process an RSA private key using an RSA structure. 
	// It handles the same formats as the PrivateKey functions but an error occurs if the private key is not RSA.
	if ((rsa = PEM_read_RSAPrivateKey(file, NULL, NULL, NULL)) == NULL)
	{
		iResultCode   =  -2;
		strResultInfo =  "PEM_read_RSA_PUBKEY err";

		fclose(file);
		return -1;
	}

	// should include <openssl/evp.h>
	// private key allocation, EVP_PKEY_free() frees up the private key
	pKey = EVP_PKEY_new();
	if (pKey == NULL)
	{
		iResultCode   =  -3;
		strResultInfo =  "EVP_PKEY_new err";

		RSA_free(rsa);
		fclose(file);
		return -1;
	}

	// should include <openssl/evp.h>
	// set the key referenced by pKey to rsa
	// return 1 for success or 0 for failure
	if (EVP_PKEY_set1_RSA(pKey, rsa) != 1)
	{
		iResultCode   =  -4;
		strResultInfo =  "EVP_PKEY_set1_RSA err";

		RSA_free(rsa);
		EVP_PKEY_free(pKey);
		fclose(file);
		return -1;
	}

	// decodebase64 cipher
	unsigned char ucCipherDecodeBase64[COMM_MAX_BUFLEN] =  {0};
	int iCipherDecodeBase64                             =  sizeof(ucCipherDecodeBase64);

	iCipherDecodeBase64 = DecodeBase64((unsigned char *)szData, ucCipherDecodeBase64, iCipherDecodeBase64, strlen(szData));
	//printf("cipher DecodeBase64 [%d:%s]\n", iCipherDecodeBase64, ucCipherDecodeBase64);


	printf("before RSA_private_decrypt: ucCipherDecodeBase64 len[%d] szClearData len[%d]\n", iCipherDecodeBase64, iClearDataLen);
	// RSA_PKCS1_OAEP_PADDING
	iClearDataLen = RSA_private_decrypt(iCipherDecodeBase64, (const unsigned char *)ucCipherDecodeBase64, 
			(unsigned char *)szClearData, rsa, RSA_PKCS1_OAEP_PADDING);
	printf("after RSA_private_decrypt: ucCipherDecodeBase64 len[%d] szClearData len[%d]\n", iCipherDecodeBase64, iClearDataLen);

	//------------------------------------------
	// debug
	printf("szClearData [%d:%s]\n", iClearDataLen, szClearData);
	//------------------------------------------

	RSA_free(rsa);
	EVP_PKEY_free(pKey);
	fclose(file);

	return 0;
}

void usage(char ** argv)
{
	printf("usage: %s "
			"calcRsaSign cleardata | "
			"verifyRsaSign cleardata sign base64 or hex | "
			"rsaEncrypt cleardata| "
			"rsaDecrypt cipherbase64\n", argv[0]);
}

int main(int argc, char ** argv)
{

	if (argc < 2)
	{
		usage(argv);
		return 0;
	}

	string strCmd         =  argv[1];
	//string strClearData =  "too many secrets";

	string strPubKeyPath     =  "./rsakey/public.pem";
	string strPrivateKeyPath =  "./rsakey/privatekey.pem";
	//string strPubKeyPath       =  "./webank_ca/res_publickey.pem";
	//string strPubKeyPath       =  "./oufei_rsakey/mi_public_key.pem";
	//string strPrivateKeyPath   =  "./oufei_rsakey/mi_private_key.pem";



	int iRet =  0;
	int iResultCode = 0;
	string strResultInfo;


	if (strCmd == "calcRsaSign")
	{
		string strClearData, strSign;
		if (argc == 3)
		{
			strClearData = argv[2];
		}
		else
		{
			usage(argv);
			return 0;
		}

		if ((iRet = calcRsaSign(strClearData.c_str(), strSign, strPrivateKeyPath, iResultCode, strResultInfo)) != 0)
		{
			printf("calcRsaSign err[%d:%d:%s]\n", iRet, iResultCode, strResultInfo.c_str());
		}
	}
	else if (strCmd == "verifyRsaSign")
	{
		string strClearData, strSign, strSignType;
		if (argc == 5)
		{
			strClearData =  argv[2];
			strSign      =  argv[3];
			strSignType  =  argv[4];
		}
		else
		{
			usage(argv);
			return 0;
		}

		if ((iRet = verifyRsaSign(strClearData.c_str(), strSign.c_str(), strSignType.c_str(), strPubKeyPath, iResultCode, strResultInfo)) != 0)
		{
			printf("verifyRsaSign err[%d:%d:%s]\n", iRet, iResultCode, strResultInfo.c_str());
		}
		else
		{
			printf("verifyRsaSign ok[%d:%d:%s]\n", iRet, iResultCode, strResultInfo.c_str());
		}
	}
	else if (strCmd == "rsaEncrypt")
	{
		string strClearData;
		if (argc == 3)
		{
			strClearData = argv[2];
		}
		else
		{
			usage(argv);
			return 0;
		}

		char szCipherData[COMM_MAX_BUFLEN] =  {0};
		int iCipherDataLen                 =  sizeof(szCipherData);

		if ((iRet = rsaEncrypt(strClearData.c_str(), strPubKeyPath, szCipherData, iCipherDataLen, iResultCode, strResultInfo)) != 0)
		{
			printf("rsaEncrypt err[%d:%d:%s]\n", iRet, iResultCode, strResultInfo.c_str());
		}
	}
	else if (strCmd == "rsaDecrypt")
	{
		string strCipherBase64;
		if (argc == 3)
		{
			strCipherBase64 = argv[2];
		}
		else
		{
			usage(argv);
			return 0;
		}

		char szClearData[COMM_MAX_BUFLEN] =  {0};
		int iClearDataLen                 =  sizeof(szClearData);

		if ((iRet = rsaDecrypt(strCipherBase64.c_str(), strPrivateKeyPath, szClearData, iClearDataLen, iResultCode, strResultInfo)) != 0)
		{
			printf("rsaDecrypt err[%d:%d:%s]\n", iRet, iResultCode, strResultInfo.c_str());
		}
	}
	else
	{
		printf("cmd invalid\n");
	}

	return 0;
}

