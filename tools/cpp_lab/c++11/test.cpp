/* 编译环境：X86_64, gcc-4.8.1
 * 测试C++11新特性
 * 2013-10-01 wcdj
 * */

#include <iostream>
#include <cstdlib>   // NULL
#include <typeinfo>  // typeid
#include <string>
#include <vector>

int size1()
{
	return 0;
}

constexpr int size2()
{
	return 0;
}

int no_carry_out_by_use_deltype()
{
	std::cout << "no carry out by use deltype\n";
	return 0;
}

int main()
{
	// 【1】long long Type
	std::cout << "【1】long long Type" << std::endl;
	int ivar        = 1;
	long lvar       = 2;
	long long llvar = 3;
	std::cout << "sizeof(int)=" << sizeof(ivar) << std::endl;
	std::cout << "sizeof(long)=" << sizeof(lvar) << std::endl;
	std::cout << "sizeof(long long)=" << sizeof(llvar) << std::endl;

	// 【2】list initialization
	std::cout << "【2】list initialization" << std::endl;
	int units_sold1 = 0;
	int units_sold2 = {0};
	int units_sold3{0};
	int units_sold4(0);

	long double ld = 3.14159265356;
	int a{ld};
	int b = {ld};
	int c(ld);
	int d = ld;
	std::cout << "a=" << a << ", b=" << b << ", c=" << c << ", d=" << d << std::endl;

	// 【3】nullptr
	// 注意：编译时需要添加-std=c++11 or -std=gnu++11选项，否则编译出错
	std::cout << "【3】nullptr" << std::endl;
	int *p1 = nullptr;  // equivalent to int *p1 = 0;
	int *p2 = 0;        // directly initializes p2 from the literal constant 0
	int *p3 = NULL;     // equivalent to int *p3 = 0;

	// 【4】constexpr variables
	std::cout << "【4】constexpr variables" << std::endl;
	constexpr int mf = 20;          // 20 is a constant expression
	constexpr int limit = mf + 1;   // mf + 1 is a constant expression
	//constexpr int sz = size1();   // error, call to non-constexpr function ‘int size1()’
	constexpr int sz = size2();     // ok only if size2 is a constexpr function

	// 【5】type alias declarations, using A = B;
	std::cout << "【5】type alias declarations" << std::endl;
	// we can define a type alias in one of two ways. Traditionally, we use a typedef
	typedef double wages;           // wages is a synonym for double
	wages gerry = 100;
	std::cout << "gerry=" << gerry << std::endl;
	// the new standard introduced a second way to define a type alias, via an alias declaration
	using SIZE_T = unsigned int;
	SIZE_T wcdj = 200;
	std::cout << "wcdj=" << wcdj << std::endl;

	// 【6】the auto Type Specifier
	// unlike type specifiers, such as double, that name a specific type, auto tells the compiler to deduce the type from the initializer. By implication, a variable that uses auto as its type specifier must have an initializer
	std::cout << "【6】the auto Type Specifier" << std::endl;
	int ivar1  = 1;
	long lvar1 = 2;
	auto item = ivar1 + lvar1;
	std::cout << "ivar1=" << ivar1 << ", typeid(ivar1)=" << typeid(ivar1).name() << std::endl;
	// 可以发现item变量是long类型
	std::cout << "auto item=" << item << ", typeid(item)=" << typeid(item).name() << std::endl;

	// len has type string:size_type
	std::string line = "My name is wcdj";
	auto len = line.size();
	std::cout << "len=" << len << ", typeid(len)=" << typeid(len).name() << std::endl;

	// 【7】the decltype Type Specifier
	// Sometimes we want to define a variable with a type that the compiler deduces from
	// an expression but do not want to use that expression to initialize the variable.
	// For such cases, the new standard introduced a second type specifier, decltype, which returns the type of its operand.
	// The compiler analyzes the expression to determine its type but does not evaluate the expression.
	std::cout << "【7】the decltype Type Specifier" << std::endl;
	decltype(no_carry_out_by_use_deltype()) int_type_var = 1;
	std::cout << "typeid(int_type_var)=" << typeid(int_type_var).name() << std::endl;

	// 【8】Range for Statement
	std::cout << "【8】Range for Statement" << std::endl;
	std::string str("some string");
	// print the characters in str one character to a line
	for (auto c : str)// for every char in str
	{
		// print the current character followed by a newline
		std::cout << c << std::endl;
	}
	std::cout << "range end" << std::endl;

	std::string s("Hello World!!!");
	// punct_cnt has the same type that s.size returns
	decltype(s.size()) punct_cnt = 0;
	// count the number of punctuation characters in s
	for (auto c : s)
	{
		if (ispunct(c)) ++punct_cnt;
	}
	std::cout << punct_cnt << " punctuation characters in " << s << std::endl;

	// 【9】 Defining a vector of vectors
	// vecotr whose elements are vectors
	// Note: In the past, we had to supply a space between the closing angle bracket of the outer
	// vector and its element type - vector<vector<int> > rather than vector<vector<>>
	std::cout << "【9】Defining a vector of vectors" << std::endl;
	std::vector<std::vector<std::string>> file;
	std::vector<std::string> f1;
	f1.push_back("wcdj");
	std::vector<std::string> f2;
	f2.push_back("gerry");
	file.push_back(f1);
	file.push_back(f2);
	

	return 0;
}
