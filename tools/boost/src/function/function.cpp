#include <boost/function.hpp> 
#include <iostream> 
#include <cstdlib> 
#include <cstring> 

struct world 
{ 
	void hello(std::ostream &os) 
	{ 
		os << "Hello, world!" << std::endl; 
	} 
}; 

int main() 
{ 
	// ¡¾×¢ÊÍ1¡¿
	boost::function<int (const char*)> f1 = std::atoi; 
	std::cout << f1("1609") << std::endl; 

	f1 = std::strlen; 
	std::cout << f1("1609") << std::endl; 

	// ¡¾×¢ÊÍ2¡¿
	try 
	{ 
		boost::function<int (const char*)> f2; 
		f2(""); 
	} 
	catch (boost::bad_function_call &ex) 
	{ 
		std::cout << ex.what() << std::endl; 
	} 

	boost::function<int (const char*)> f3; 
	if (f3.empty())
	{
		std::cout << "f3 is empty object, should not use" << std::endl;
	}

	// ¡¾×¢ÊÍ3¡¿
	boost::function<void (world*, std::ostream&)> f4 = &world::hello; 
	world w; 
	f4(&w, boost::ref(std::cout)); 

	return 0;
} 
/*
mba:bind gerryyang$ ./function 
1609
4
call to empty boost::function
f3 is empty object, should not use
Hello, world!
* */
