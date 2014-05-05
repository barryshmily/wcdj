#include <iostream> 
#include <vector> 
#include <algorithm> 
#include <boost/lambda/lambda.hpp> 
#include <boost/lambda/if.hpp> 

void print(int i) 
{ 
	std::cout << i << std::endl; 
} 

int main() 
{ 
	std::vector<int> v; 
	v.push_back(1); 
	v.push_back(3); 
	v.push_back(2); 

	// 【注释1】
	std::for_each(v.begin(), v.end(), print); 
	std::cout << "--------------" << std::endl;
	
	// 【注释2】
	std::for_each(v.begin(), v.end(), std::cout << boost::lambda::_1 << "\n"); 
	std::cout << "--------------" << std::endl;

	// 只将大于1的元素写出到标准输出流
	// 【注释3】
	std::for_each(v.begin(), v.end(), 
			boost::lambda::if_then(boost::lambda::_1 > 1, 
				std::cout << boost::lambda::_1 << "\n")); 

	return 0;
} 
/*
mba:bind gerryyang$ ./lambda 
1
3
2
--------------
1
3
2
--------------
3
2 
* */
