#include <stdio.h>
#include <string.h>
#include <string>
using namespace std;

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("usage: %s str\n", argv[0]);
		return 1;
	}

	char buf[5] = {0};
	string str = argv[1];

	if (sizeof(buf) > str.size())
	{
		snprintf(buf, sizeof(buf), "%s", str.c_str());
	}
	else
	{
		const char *beg = str.c_str();
		unsigned int gap = 0;
		gap += str.size() - sizeof(buf) + 1;
		
		snprintf(buf, sizeof(buf), "%s", string(beg + gap).c_str());
	}

	printf("buf [%s]\n", buf);

	return 0;
}
