#include <stdio.h>
#include <string.h>
#include <string>

using namespace std;

int main()
{
	string str   = "140924";
	int order    = 12345;

	const int maxlen     = 10;
	char buf[maxlen + 1] = {0};

	snprintf(buf, sizeof(buf), "%.*d", maxlen, order);
	printf("buf[%s]\n", buf);

	str += buf;
	printf("str[%s]\n", str.c_str());

	return 0;
}
/*
output:
buf[0000012345]
str[1409240000012345]
*/
