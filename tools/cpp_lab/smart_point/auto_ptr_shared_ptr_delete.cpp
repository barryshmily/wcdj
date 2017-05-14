#include <iostream>
#include <string>
#include <memory>

using namespace std;

int main()
{
	std::auto_ptr<std::string> aps(new std::string[10]);
	std::shared_ptr<int> spi(new int[1024]);
}
