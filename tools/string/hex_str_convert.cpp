#include <cstdio>
#include <cstring>
#include <string>
using namespace std;

int strtohex(std::string &src, std::string &dst);
int hextostr(std::string &src, std::string &dst);


int main()
{
	std::string strDefaultKey = "^8W~4'>IDxS`Jx?2";
	printf("DefaultKey: %s\n", strDefaultKey.c_str());

	std::string strHexKey;
	strtohex(strDefaultKey, strHexKey);
	printf("HexKey: %s\n", strHexKey.c_str());

	std::string strStrKey;
	hextostr(strHexKey, strStrKey);
	printf("StrKey: %s\n", strStrKey.c_str());

	return 0;
}

int strtohex(std::string &src, std::string &dst)
{
	dst = "";
	for (size_t i = 0; i < src.size(); ++i)
	{
		char szch[3] = {0};
		sprintf(szch, "%x", src[i]);

		dst += szch;
	}

	return 0;
}

int hextostr(std::string &src, std::string &dst)
{
	dst = "";
	for (size_t i = 0; i < src.size(); i += 2)
	{
		int c = 0;
		sscanf(src.c_str() + i, "%2x", &c);
		dst += (char)c;   
	}

	return 0;
}

/* g++ -o hex_str_convert hex_str_convert.cpp
 *
 * output:
 * DefaultKey: ^8W~4'>IDxS`Jx?2
 * HexKey: 5e38577e34273e49447853604a783f32
 * StrKey: ^8W~4'>IDxS`Jx?2
 * */

