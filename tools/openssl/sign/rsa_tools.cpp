#include "rsa_tools.h"

namespace RSATOOLS 
{

	void str2Hex(unsigned char* buf, int& len, string& hex)
	{
		for (int i = 0; i < len; ++i)
		{
			char szTmp[3] = {0};
			snprintf(szTmp, sizeof(szTmp), "%02X", buf[i]);
			hex += szTmp;
		}
	}

	int hex2Str(std::string &src, std::string &dst)
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
	int calcRsaSign(const char * szData, string& strSign, const char * szSignType, string& strPrivateKeyPath, int& iResultCode, string& strResultInfo)
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
		// hex sign
		string strSignHex;

		if (string(szSignType) == "base64")
		{
			iSignBase64 = EncodeBase64((unsigned char *)ucSign, ucSignBase64, iSignBase64, iSign);
			strSign = (char *)ucSignBase64;
		}
		else if (string(szSignType) == "hex")
		{
			str2Hex(ucSign, iSign, strSignHex);
			strSign = strSignHex;
		}
		else
		{
			iResultCode   =  -6;
			strResultInfo =  "SignType should be base64 or hex";
			return -1;
		}


		free(ucSign);
		RSA_free(rsa);
		EVP_PKEY_free(pKey);
		fclose(file);
		EVP_MD_CTX_cleanup(&ctx);

		return 0;
	}

	// use private.key to calc sign
	int calcRsaSignFromStr(const char * szData, string& strSign, const char * szSignType, string& strPrivateKey, int& iResultCode, string& strResultInfo)
	{
		int iRet = 0;

		RSA *rsa       =  NULL;
		BIO *bio       =  NULL;
		EVP_PKEY *pKey =  NULL;


		int keyLen = strPrivateKey.size();
		for(int i = 64; i < keyLen; i += 64)
		{
			if (strPrivateKey[i] != '\n')
			{
				strPrivateKey.insert(i, "\n");
			}
			++i;
		}

		strPrivateKey.insert(0, "-----BEGIN RSA PRIVATE KEY-----\n");
		strPrivateKey.append("\n-----END RSA PRIVATE KEY-----\n");

		char * szPrivateKey = const_cast<char *>(strPrivateKey.c_str());

		// 从字符串读取RSA私钥
		if ((bio = BIO_new_mem_buf(szPrivateKey, strPrivateKey.length())) == NULL)       
		{
			char errBuf[512] = {0};
			ERR_error_string_n(ERR_get_error(), errBuf, sizeof(errBuf));

			iResultCode   =  1;
			strResultInfo =  std::string("BIO_new_mem_buf err[") + errBuf + "]";;

			return -1;
		}

		// 从bio结构中得到RSA结构
		rsa = PEM_read_bio_RSAPrivateKey(bio, NULL, NULL, NULL);   
		if (NULL == rsa)
		{
			char errBuf[512] = {0};
			ERR_error_string_n(ERR_get_error(), errBuf, sizeof(errBuf));

			iResultCode   =  2;
			strResultInfo =  std::string("PEM_read_bio_PrivateKey err[") + errBuf + "]";

			return -1;
		}
		BIO_free(bio);

		// should include <openssl/evp.h>
		// private key allocation, EVP_PKEY_free() frees up the private key
		pKey = EVP_PKEY_new();
		if (pKey == NULL)
		{
			iResultCode   =  -3;
			strResultInfo =  "EVP_PKEY_new err";

			RSA_free(rsa);
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
			EVP_MD_CTX_cleanup(&ctx);
			return -1;
		}

		// encodebase64 sign 
		unsigned char ucSignBase64[COMM_MAX_BUFLEN] =  {0};
		int iSignBase64                             =  sizeof(ucSignBase64);
		// hex sign
		string strSignHex;

		if (string(szSignType) == "base64")
		{
			iSignBase64 = EncodeBase64((unsigned char *)ucSign, ucSignBase64, iSignBase64, iSign);
			strSign = (char *)ucSignBase64;
		}
		else if (string(szSignType) == "hex")
		{
			str2Hex(ucSign, iSign, strSignHex);
			strSign = strSignHex;
		}
		else
		{
			iResultCode   =  -6;
			strResultInfo =  "SignType should be base64 or hex";
			return -1;
		}

		free(ucSign);
		RSA_free(rsa);
		EVP_PKEY_free(pKey);
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
			hex2Str(strSignHex, strSign);
			iSign = strSign.length();
			memcpy(ucSign, strSign.c_str(), iSign);
		}
		else
		{
			iResultCode   =  -5;
			strResultInfo =  "SignType should be base64 or hex";
			return -1;
		}

		// should include <openssl/rsa.h>
		// RSA_verify() verifies that the signature sigbuf of size siglen matches a given message digest m of size m_len. 
		// type denotes the message digest algorithm that was used to generate the signature. rsa is the signer's public key
		iRet = RSA_verify(NID_sha1, ucDigest, iDigest, ucSign, iSign, rsa);
		if (iRet != 1)
		{
			iResultCode   =  -6;
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

	// use public.key to verify sign
	int verifyRsaSignFromStr(const char * szData, const char * szSign, const char * szSignType, string& strPubKey, int& iResultCode, string& strResultInfo)
	{ 
		int iRet = 0;

		RSA *rsa       =  NULL;
		BIO *bio       =  NULL;
		EVP_PKEY *pKey =  NULL;

		int keyLen = strPubKey.size();
		for(int i = 64; i < keyLen; i += 64)
		{
			if (strPubKey[i] != '\n')
			{
				strPubKey.insert(i, "\n");
			}
			++i;
		}

		strPubKey.insert(0, "-----BEGIN PUBLIC KEY-----\n");
		strPubKey.append("\n-----END PUBLIC KEY-----\n");

		char * szPubKey = const_cast<char *>(strPubKey.c_str());

		// 从字符串读取RSA公钥
		if ((bio = BIO_new_mem_buf(szPubKey, strPubKey.length())) == NULL)       
		{
			char errBuf[512] = {0};
			ERR_error_string_n(ERR_get_error(), errBuf, sizeof(errBuf));

			iResultCode   =  1;
			strResultInfo =  std::string("BIO_new_mem_buf err[") + errBuf + "]";;

			return -1;
		}

		// 从bio结构中得到RSA结构
		rsa = PEM_read_bio_RSA_PUBKEY(bio, NULL, NULL, NULL);   
		if (NULL == rsa)
		{
			char errBuf[512] = {0};
			ERR_error_string_n(ERR_get_error(), errBuf, sizeof(errBuf));

			iResultCode   =  2;
			strResultInfo =  std::string("PEM_read_bio_RSA_PUBKEY err[") + errBuf + "]";

			return -1;
		}
		BIO_free(bio);

		// should include <openssl/evp.h>
		// private key allocation, EVP_PKEY_free() frees up the private key
		pKey = EVP_PKEY_new();
		if (pKey == NULL)
		{
			iResultCode   =  -3;
			strResultInfo =  "EVP_PKEY_new err";

			RSA_free(rsa);
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
			hex2Str(strSignHex, strSign);
			iSign = strSign.length();
			memcpy(ucSign, strSign.c_str(), iSign);
		}
		else
		{
			iResultCode   =  -5;
			strResultInfo =  "SignType should be base64 or hex";
			return -1;
		}

		// should include <openssl/rsa.h>
		// RSA_verify() verifies that the signature sigbuf of size siglen matches a given message digest m of size m_len. 
		// type denotes the message digest algorithm that was used to generate the signature. rsa is the signer's public key
		iRet = RSA_verify(NID_sha1, ucDigest, iDigest, ucSign, iSign, rsa);
		if (iRet != 1)
		{
			iResultCode   =  -6;
			strResultInfo =  "RSA_verify err";

			RSA_free(rsa);
			EVP_PKEY_free(pKey);
			EVP_MD_CTX_cleanup(&ctx);
			return -1;
		}

		RSA_free(rsa);
		EVP_PKEY_free(pKey);
		EVP_MD_CTX_cleanup(&ctx);

		return 0;
	}

	// rsa encrypt
	int rsaEncrypt(const char * szData, string& strPubKeyPath, char * szCipherData, int& iCipherDataLen, const char * szCipherType, int& iResultCode, string& strResultInfo)
	{

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

		// RSA_PKCS1_OAEP_PADDING
		iCipherDataLen = RSA_public_encrypt(strlen(szData), (const unsigned char *)szData, (unsigned char *)szCipherData, rsa, RSA_PKCS1_OAEP_PADDING);

		// encodebase64 cipher
		unsigned char ucCipherBase64[COMM_MAX_BUFLEN] =  {0};
		int iCipherBase64                             =  sizeof(ucCipherBase64);
		// hex cipher
		string strCipherDataHex;

		if (string(szCipherType) == "base64")
		{
			iCipherBase64 = EncodeBase64((unsigned char *)szCipherData, ucCipherBase64, iCipherBase64, iCipherDataLen);
		}
		else if (string(szCipherType) == "hex")
		{
			str2Hex((unsigned char *)szCipherData, iCipherDataLen, strCipherDataHex);
		}
		else
		{
			iResultCode   =  -5;
			strResultInfo =  "CipherType should be base64 or hex";
			return -1;
		}


		RSA_free(rsa);
		EVP_PKEY_free(pKey);
		fclose(file);

		return 0;
	}

	// rsa decrypt
	int rsaDecrypt(const char * szCipher, const char * szCipherType, string& strPrivateKeyPath, char * szClearData, int& iClearDataLen, int& iResultCode, string& strResultInfo)
	{

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
		unsigned char ucCipher[COMM_MAX_BUFLEN] =  {0};
		int iCipher                             =  sizeof(ucCipher);

		if (string(szCipherType) == "base64")
		{
			iCipher = DecodeBase64((unsigned char *)szCipher, ucCipher, iCipher, strlen(szCipher));
		}
		else if (string(szCipherType) == "hex")
		{
			string strSignHex = szCipher;
			string strCipher;
			hex2Str(strSignHex, strCipher);
			iCipher = strCipher.length();
			memcpy(ucCipher, strCipher.c_str(), iCipher);
		}

		// RSA_PKCS1_OAEP_PADDING
		iClearDataLen = RSA_private_decrypt(iCipher, (const unsigned char *)ucCipher, 
				(unsigned char *)szClearData, rsa, RSA_PKCS1_OAEP_PADDING);


		RSA_free(rsa);
		EVP_PKEY_free(pKey);
		fclose(file);

		return 0;
	}

}

