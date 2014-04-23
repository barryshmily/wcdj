#include <cstdio>
#include <cstring>
#include <string>
using namespace std;

// http://en.wikipedia.org/wiki/Urlencode
string encodeURIComponent(const string &s)
{
	string sResult = "";
	char c;
	const char *digits = "0123456789ABCDEF";

	for (size_t i = 0; i < s.size(); i++)
	{
		c = s[i];

		if (('0'<=c && c<='9') || ('a'<=c && c<='z') || ('A'<=c && c<='Z') || strchr("!*()", c) != NULL)
		{
			sResult += c;
		} 
		else
		{
			sResult += "%";
			sResult += digits[(c >> 4) & 0x0f];
			sResult += digits[c & 0x0f];
		}
	}

	return sResult;
}

int main(int argc, char **argv)
{
	string str = "http://www.gerryyang.tk/cgi-bin/a=b&c=d";
	printf("str[%s]\nurlencode str[%s]\n", str.c_str(), encodeURIComponent(str).c_str());

	return 0;
}
/* g++ urlencode.cpp -o urlencode
 * output:
 * str[http://www.gerryyang.tk/cgi-bin/a=b&c=d]
 * urlencode str[http%3A%2F%2Fwww%2Egerryyang%2Etk%2Fcgi%2Dbin%2Fa%3Db%26c%3Dd]
 * */
