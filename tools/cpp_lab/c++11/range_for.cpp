#include <iostream>
#include <string>
using namespace std;

int main()
{
	std::string str("some string");
	// print the characters in str one character to a line
	for (auto c : str)// for every char in str
	{
		// print the current character followed by a newline
		std::cout << c << std::endl;
	}

	return 0;
}
