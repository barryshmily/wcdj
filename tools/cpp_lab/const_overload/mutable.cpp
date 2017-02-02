#include<stdio.h>
#include<iostream>
#include<string>
#include<string.h>

class TextBlock {
public:
	TextBlock() : lengthIsValid(false)
	{
	}
	TextBlock(char* lhs) : lengthIsValid(false)
	{
		pText = lhs;
	}
public:
	std::size_t length() const
	{
		if (!lengthIsValid) {
			printf("do strlen... ");
			textLength = std::strlen(pText);    // error? 在const成员函数内不能修改non-static成员变量
			lengthIsValid = true;               // 同上
		}
		return textLength;
	}

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

	mutable std::size_t textLength;    // 最近一次计算的文本区域块长度
	mutable bool lengthIsValid;        // 目前的长度是否有效
};

int main()
{
	char name[] = "gerry";
	const TextBlock ctb(name);
	std::cout << ctb[0] << std::endl;         // 调用const TextBlock::operator[]
	std::cout << "length: " << ctb.length() << std::endl;

	char* pc = &ctb[0];
	*pc = 'J';
	std::cout << ctb[0] << std::endl;         // 调用const TextBlock::operator[]
	std::cout << "length: " << ctb.length() << std::endl;

	return 0;
}
/*
$./mutable 
g
length: do strlen... 5
J
length: 5
 */
