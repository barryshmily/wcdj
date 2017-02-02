#include<stdio.h>
#include<iostream>
#include<string>

class TextBlock {
public:
	TextBlock()
	{
	}
	TextBlock(char* lhs)
	{
		pText = lhs;
	}
public:
	// operator[] for const object
	char& operator[] (std::size_t position) const 
	{
		return pText[position];
	}

#if 0
	// operator[] for non-const object
	char& operator[] (std::size_t position)
	{
		return pText[position];
	}
#endif

private:
	char* pText;
};

int main()
{
	char name[] = "gerry";
	const TextBlock ctb(name);
	std::cout << ctb[0] << std::endl;         // 调用const TextBlock::operator[]

	char* pc = &ctb[0];
	*pc = 'J';
	std::cout << ctb[0] << std::endl;         // 调用const TextBlock::operator[]

	return 0;
}
