#include <iostream>
#include <string>
#include <algorithm>

int main()
{
	std::string str = "aBcDeF";
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	std::cout << str << std::endl;

	return 0;
}
// output: abcdef
