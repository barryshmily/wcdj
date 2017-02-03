#include<stdio.h>
#include<iostream>
#include<string>

class TextBlock {
public:
	TextBlock()
	{
	}
	TextBlock(const char* lhs)
	{
		text = lhs;
	}
public:
	// operator[] for const object
	const char& operator[] (std::size_t position) const 
	{
		// bounds checking
		// log access data
		// verify data integrity
		// ...

		return text[position];
	}

	// operator[] for non-const object
	char& operator[] (std::size_t position)
	{
		// bounds checking
		// log access data
		// verify data integrity
		// ...

		return text[position];
	}

private:
	std::string text;
};

int main()
{
	TextBlock tb("gerry");
	std::cout << tb[0] << std::endl;         // 调用non-const TextBlock::operator[]

	const TextBlock ctb("yang");             // 调用const TextBlock::operator[]
	std::cout << ctb[0] << std::endl;

	return 0;
}
