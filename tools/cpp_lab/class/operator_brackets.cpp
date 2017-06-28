#include <iostream>
#include <string>

class Widget {
public:
	explicit Widget(int rhs) : a(rhs)
	{
	}

	Widget& operator=(const Widget& rhs) // 返回类型是个reference，指向当前对象
	{
		a = rhs.a;
		return *this;   // 返回左侧对象
	}

	Widget& operator=(int rhs) // 此函数也适用，即使此操作符的参数类型不符协定
	{
		a = rhs;
		return *this;
	}

	Widget& operator+=(const Widget& rhs)
	{
		this->a += rhs.a;
		return *this;
	}

	// operator brackets
	std::string operator()() const
	{
		return std::to_string(a);
	}

	void print()
	{
		std::cout << a << "\n";
	}

private:
	int a;
};

int main()
{
	Widget w(1);
	w.print();

	Widget w2(2);
	w = w2;
	w.print();

	w = 100;
	w.print();

	w += w2;
	w.print();

	std::string str = w();
	std::cout << "str: " << str << std::endl;
}
/*
g++ -std=c++11 -o operator_brackets operator_brackets.cpp
1
2
100
102
str: 102
*/
