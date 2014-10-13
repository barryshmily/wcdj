// tencent_des.cpp : Defines the entry point for the console application.
//


#include "baseutils.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    //std::string cleartext = "13807198784216434";
	//std::string key = "5B0A213A78504BBFD13F3D8534A4C75D";
    std::string cleartext = "too many secrets";
	std::string key = "123";

	std::string ciphertext = baseutils::des_pkcs_ecb_enc(cleartext, key);
	std::string originaltext = baseutils::des_pkcs_ecb_dec(ciphertext, key);

	cout << "cleartext[" << cleartext << "] key[" << key << "]" << endl;
	cout << "ciphertext[" << ciphertext << "] originaltext[" << originaltext << "]" << endl;

	return 0;
}

