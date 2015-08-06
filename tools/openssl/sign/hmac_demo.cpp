#include <stdio.h>
#include <string>
#include <string.h>
#include "hmac_tools.h"
using namespace std;

std::string hmac_sha1_base64(std::string &key, std::string &data);
std::string hmac_sha1_hex(std::string &key, std::string &data);

void usage(char ** argv)
{
	printf("usage: %s key data\n", argv[0]);
}

int main(int argc, char ** argv)
{

	if (argc < 2)
	{
		usage(argv);
		return 0;
	}

	string key  =  argv[1];
	string data =  argv[2];
	string sign;

	printf("sign[%s]\n", hmac_sha1_base64(key, data).c_str());

	return 0;
}

