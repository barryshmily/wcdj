#include<iostream>
#include<string>

class foo
{
public:
	foo() {
		std::cout << "foo()\n";
	}
	foo(const char* rhs) {
		std::cout << "foo(const char*)\n";
		str = rhs;
	}

public:
	std::string str;
};

foo test_rvo()
{
	foo obj("hello");
	return obj;
}

int main()
{
	foo obj = test_rvo();

	std::cout << obj.str << std::endl;
	return 0;
}
