#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
#include <ios>

using namespace std;


void decimal2Hex(unsigned int decimal, string& hex)
{
	std::stringstream ss;
	ss << std::hex << decimal;
	hex = ss.str();

	printf("decimal[%u] -> hex[%s]\n", decimal, hex.c_str());
}

void hex2Decimal(const char *hex, unsigned int decimal)
{
	std::stringstream ss;
	ss << hex;
	ss >> std::hex >> decimal;

	printf("hex[%s] -> decimal[%u]\n", hex, decimal);
}

int main()
{
	string str;

	decimal2Hex(255, str);
	decimal2Hex(256, str);
	decimal2Hex(2147483647, str);
	decimal2Hex(2147483648U, str);

	unsigned int deci;
	hex2Decimal("ff", deci);
	hex2Decimal("100", deci);
	hex2Decimal("7fffffff", deci);
	hex2Decimal("80000000", deci);

	return 0;
}
/*
output:
decimal[255] -> hex[ff]
decimal[256] -> hex[100]
decimal[2147483647] -> hex[7fffffff]
decimal[2147483648] -> hex[80000000]
hex[ff] -> decimal[255]
hex[100] -> decimal[256]
hex[7fffffff] -> decimal[2147483647]
hex[80000000] -> decimal[2147483648]
*/
