#include <stdio.h>
#include <openssl/des.h>

int main(int argc,char **argv)
{
	DES_cblock key;
	DES_string_to_key("1234567", &key);

	DES_key_schedule schedule;
	DES_set_key_checked(&key, &schedule); 

	const_DES_cblock input = "1234567";
	DES_cblock output;

	printf("cleartext[%s]\n", input);

	DES_ecb_encrypt(&input, &output, &schedule, DES_ENCRYPT);
	printf("Encrypted!\n");

	printf("ciphertext:");
	int i;
	for (i = 0; i < sizeof(input); ++i)
		printf("%02X", output[i]);
	printf("\n");

	DES_ecb_encrypt(&output, &input, &schedule, DES_DECRYPT);
	printf("Decrypted!\n");
	printf("cleartext[%s]\n", input);

	return 0;
} 
