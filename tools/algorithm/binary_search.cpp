#include <iostream>
#include <string>

int main(int argc, char **argv)
{
	std::cout << "input a char:";
	char sought;
	std::cin >> sought;

	std::string text = "123456789";

	auto beg = text.begin();
	auto end = text.end();
	auto mid = beg + (end - beg) / 2;// init
	std::cout << "init *mid " << *mid << std::endl; 

	while (mid != end && *mid != sought) {
		if (sought < *mid) {
			end = mid;
		} else {
			beg = mid + 1;
		}
		mid = beg + (end - beg) / 2;// update
		std::cout << "update *mid " << *mid << std::endl; 
	}

	if (mid != end) {
		std::cout << "find " << sought << " at positon " << mid - text.begin() << std::endl;
	} else {
		std::cout << "not find " << sought << " from " << text << std::endl;
	}

	return 0;
}
/*
 g++ -std=c++11 binary_search.cpp -o binary_search

output:
input a char:1
init *mid 5
update *mid 3
update *mid 2
update *mid 1
find 1 at positon 0

input a char:0
init *mid 5
update *mid 3
update *mid 2
update *mid 1
update *mid 1
not find 0 from 123456789

 */

