#include <boost/regex.hpp>   
#include <iostream>  
//#pragma comment(lib,"libboost_regex-vc90-mt-sgd-1_44.lib")  
int main()   
{   
	std::string s = "wcdj gerry";   
	boost::regex expr("//w+//s//w+");   
	// [1] regex_match  
	std::cout << boost::regex_match(s, expr) << std::endl;// 1  
	// [2] regex_search  
	expr = "wcdj";  
	std::cout << boost::regex_search(s.begin(), s.end(), expr) << std::endl;// 1  
	expr = "(//w+)//s(//w+)";   
	boost::smatch what;   
	if (boost::regex_search(s, what, expr))   
	{   
		std::cout << what[0] << std::endl;// wcdj gerry   
		std::cout << what[1] << " " << what[2] << std::endl; // wcdj gerry  
	}  
	// [3] regex_replace  
	expr = "//s";   
	std::string fmt("_");   
	std::cout << boost::regex_replace(s, expr, fmt) << std::endl;// wcdj_gerry   
	expr = "(//w+)//s(//w+)";   
	fmt = "//2 //1";   
	std::cout << boost::regex_replace(s, expr, fmt) << std::endl;// gerry wcdj  
	// [4]  
	/* 
	   boost::regex_constants::format_literal标志作为第四参数传递给函数boost::regex_replace()，从而抑制了格式参数中对特殊字符的处理 
	   */  
	expr = "(//w+)//s(//w+)";   
	fmt = "//2 //1";   
	std::cout << boost::regex_replace(s, expr, fmt, boost::regex_constants::format_literal) << std::endl;// /2 /1   

	return 0;  
}   
