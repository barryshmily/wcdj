#include <boost/signal.hpp> 
//#include <boost/signals2.hpp> 
#include <iostream> 

void func() 
{ 
	std::cout << "Hello, world!" << std::endl; 
} 

int main() 
{ 
	boost::signal<void ()> sig; 
	sig.connect(func); 
	sig(); 
} 

