#include <iostream>
#include <cstring>
#include <string>
using namespace std;


unsigned long time33(char const *str, int len) 
{ 
	unsigned long  hash = 0; 
	for (int i = 0; i < len; ++i) 
	{ 
		hash = ((hash << 5) + hash) + (unsigned long)str[i]; 
	} 
	return hash; 
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		cout << "usage: " << argv[0] << " num" << endl;
		exit(1);
	}

	string strUin = argv[1];
	cout << "input uin:" << strUin << endl;
	unsigned long liHash = time33(strUin.c_str(), strlen(strUin.c_str()));
	char szTailNum[12] = {0};
	snprintf(szTailNum, sizeof(szTailNum), "%.3ld", liHash%1000);

	cout << "time33 result: " << szTailNum << endl;

	return 0;

}
// g++ -o time33 time33.cpp 
