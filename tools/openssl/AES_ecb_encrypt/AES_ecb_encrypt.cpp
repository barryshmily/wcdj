#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <openssl/aes.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/md5.h>
#include "base64.h"


void decrypt()
{

	const char *msg = "m1Kby8AyZfzj/VeRE/wBNGy28yVCKFHxXbJAhb64AHw9CeScnH3tR3SfvV7/lWDk5MYl9W6NydzNxGDSxGn4fi5ltbNb2Bs+mz+6yLo/CNHkT3/hDoYL7KLLMNlLfcVBDWFlEBjUeioDIY6NHdj+Zq7G42gJVLZsIgRicRfXruevYXLynsbvNJa3KeFhV1/dLa86C9iG4EjaKwT4fwmtkZkbRpzxBUd24fsJI1ng5lInjGSf1w5lnv3UgySOTWbSvUa96VbwvL8+cY+cX7hKg1nM5ZO8dS4zIpLl6NAlFwVIiJXCDaKepWVNj1+pMVSZhNgN1bXkW8y9pWTX7ROipb2XJdNcTdTQNWGBsqfqEKKnaXsIeCx4p9eERyBiMKFa95u7TVwEjUz1ehuPHMFgMPpXeyvKC1fH8mbbuVD4krHdqjXj6JlympggrFMoZ2amtjd80yBHNPaoCxd0K9s/SJjRuIyExET5sGSAA9kMO0xIeqoeg9x+o1KFdw+I9zn+DUkgF44H2Rur7GXpZ2vrs8k0T6OEVX59qGXHTFkxNcgF7dyYSayN2RJ1XH2fBWYgmwJ1AlglGr5sZp/eq0uqTDK423E4rCpykngXPnZ3MdasLIujP2QiCIOC7qVB9JOMgYLBe+Zb5MfP8KmZn+f5P917iBf7RQ0RqLq3HSX0bCFijxGp1k6RSHtyGlb6MUoLhMK5pdia5g6wdfvmXGlKgFy6CA6LijYG7q3EImT0aLupFsrbzH54+DwiokJgj2fC5nHIrAlo9XxAUh4jfYXW6DuBq8KuYTBGnd3luacXOrqd7MoRR1Ktfd0Liof4k7RYXZHGj5M1W4dowOnunzn3ye6Vt3nsAVkoXrtMpdkmZg2Tb61W0d5JyjzCkLv+XnDMVkRTx1ps25Ozivca4B5VZKsaKk3qKHKmyJ678WIufXPqbsEHGFE2+Oi8xMi5si6tyGRSPqTLB2U8T+qQ1ShkknDq/B9pQIId+D7OyDKYRAnHv++/TFw1ajuUHSXKy2QMUeBFm7rzWaphUVlFjXuxNFjQFqOlU9BpOwvRuFBSdno=";
	std::string key = "dfecba73e08312514b657f109135b253";

	//const char *msg = "YYwp8C48th0wnQzTqeI+41pflB26v+smFj9z6h9RPBgxTyZyxc+4YNEz7QEgZNWj/6rIb2MfyWMZmCc41CfjKSssoSZPXxOhUayb6KvNSZ1p6frOX1PDWzhyruXK7ouNND+gDsG4yZ0XXzsL4/pYNwLLba/71QrnkJ/BHcByk4EXnglju5DLup9pJQSnTxjomI9Rxu57m9jg5lLQFxMWXyeASZJNvof0ulnHlWJswS4OxKOkmW7VEyKyLGV6npoOm03Qsx2wkRxLsSa9gPpg4hdaReeUqh1FMbm7aWjyrVYT/MEZWg98p4GomEIYvz34XfDncTezX4bf/ZiSLXt79aE1/YTZrYfymXeCrGjlbe0rg/T2ezJHAC870u2vsVbY1/KcE2A443N+DEnAziXlBQ1AeWq3Rqk/O6/TMM0lomzgctAOiAMg+bh5+Gu1ubA9O3E+vehULydD5qx2o6i3+qA9ORbH415NyRrQdeFq5vmCiRikp5xYptWiGZA0tkoaLKMPQ4ndE5gWHqiBbGPfULZWokI+QjjhhBmwgbd6J0VqpRorwOuzC/BHdkP72DCdNcm7IDUpggnzBIy0+seWIkcHEryKjge3YDHpJeQCqrAH0CgxXHDt1xtbQbST1VqFyuhPhUjDXMXrknrGPN/oE1t0rLRq+78cI+k8xe5E6seeUXQsEe8r3358mpcDYSmXWSXVZxK6er9EF98APqHwcndyEJD2YyCh/mMVhERuX+7kjlRXSiNUWa/Cv/XAKFQuvUYA5ea2eYWtPRHa4DpyuF1SNsaqVKfgqKXZrJHfAgslVpSVqUpX4zkKszHF4kwMZO3M7J1P94Mxa7Tm9mTOJePOoHPXeEB+m9rX6pSfoi3mJDQ5inJ+Vc4gOkg/Wd/lqiy6TTyP/dHDN6/v+AuJx5AXBo/2NDD3fWhHjkqEKIuARr2ClZt9ZRQO4HkXdZo7CN06sGCHk48Tg8PmxnxKcMZm7Aoquv5yMIM2gWSWIRJhwJ8cUpafIHc+GesDlbF6Zbt+/KXkafJAQq2RklEN+WvZ/zFz113EPgWPjp16TwBoziq96MMekvWKY/vdhjol8VFtGH9F61Oy1Xwf6DJtPw==";
	//std::string key = "ziR0QKsTUfMOuochC9RfCdmfHECorQAP";

	std::string msg_decode = base64_decode(msg);

	std::string md5_key;
	unsigned char md[16] = {0};
	char tmp[8] = {0};
	MD5((unsigned char *)key.c_str(), key.length(), md);
	for (int i = 0; i < 16; ++i) {
		sprintf(tmp, "%02x", md[i]);
		md5_key += tmp;
	}
	std::cout << "md5_key:" << md5_key << "\n";

#if defined EVP_INF
	EVP_CIPHER_CTX *ctx;
	int len;
	int plaintext_len;


	/* Create and initialise the context */
	if(!(ctx = EVP_CIPHER_CTX_new())) {
		std::cout << "EVP_CIPHER_CTX_new err\n";
		exit(1);
	}

	/* Initialise the decryption operation. IMPORTANT - ensure you use a key
	 * and IV size appropriate for your cipher
	 * In this example we are using 256 bit AES (i.e. a 256 bit key). The
	 * IV size for *most* modes is the same as the block size. For AES this
	 * is 128 bits */
	unsigned char *iv = NULL;
	if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_ecb(), NULL, (unsigned char *)md5_key.c_str(), iv)) {
		std::cout << "EVP_DecryptInit_ex err\n";
		exit(1);
	}

	unsigned char plaintext[2048] = {0};
	unsigned char * ciphertext = (unsigned char *)msg_decode.c_str();
	int ciphertext_len = msg_decode.length();
	/* Provide the message to be decrypted, and obtain the plaintext output.
	 * EVP_DecryptUpdate can be called multiple times if necessary
	 */
	if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)) {
		std::cout << "EVP_DecryptUpdate err\n";
		exit(1);
	}
	plaintext_len = len;

	/* Finalise the decryption. Further plaintext bytes may be written at
	 * this stage.
	 */
	if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) {
		std::cout << "EVP_DecryptFinal_ex err\n";
		exit(1);
	}
	plaintext_len += len;

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);

	plaintext[plaintext_len] = '\0';
	std::cout << "plaintext_len: " << plaintext_len << " plaintext: " << plaintext << "\n";


#elif defined AES_INF
	AES_KEY dec_key;

	int msg_decode_len = msg_decode.length();
	std::cout << "msg_decode_len:" << msg_decode_len << "\n";
	int enc_len = ((msg_decode_len + AES_BLOCK_SIZE) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
	std::cout << "enc_len:" << enc_len << "\n";

	char *res = new char[enc_len];
	memset(res, 0x0, enc_len);

	unsigned char *input = new unsigned char[enc_len];
	memset(input, 0x0, enc_len);
	memcpy(input, msg_decode.c_str(), msg_decode_len);


	AES_set_decrypt_key((unsigned char *)md5_key.c_str(), 256, &dec_key);

	for (int i = 0; i < msg_decode_len - 15 ; i += AES_BLOCK_SIZE) {
		//AES_ecb_encrypt((unsigned char *)msg_decode.c_str() + i, (unsigned char *)res + i, &dec_key, AES_DECRYPT); // ok
		AES_decrypt((unsigned char *)msg_decode.c_str() + i, (unsigned char *)res + i, &dec_key); // ok
	}

	std::string str_res = res;
	//std::cout << "size:" << str_res.length() << " str_res:" << str_res << "\n";
	str_res.erase(str_res.find_last_not_of("\x0c") + 1);
	std::cout << "size:" << str_res.length() << " 2str_res:" << str_res << "\n";
	std::cout << "-----------------\n";
	//for (int i = 0; i != str_res.length(); ++i) {
	//	printf("%02x", str_res[i]);
	//}
	delete[] res;
#endif


}

int main() {

	decrypt();
	return 0;

}
