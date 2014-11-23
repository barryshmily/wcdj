#include <iostream> 
#include <vector> 
#include <algorithm> 
#include <boost/bind.hpp>

void print(int i) 
{ 
  std::cout << i << std::endl; 
} 

void add(int i, int j) 
{ 
  std::cout << i + j << std::endl; 
} 

bool compare(int i, int j) 
{ 
  return i > j; 
} 

void print_vec(std::vector<int> &v)
{
	std::vector<int>::iterator iter;
	for (iter = v.begin(); iter != v.end(); ++iter)
	{
		print(*iter);
	}
}

int main() 
{ 
  std::vector<int> v; 
  v.push_back(1); 
  v.push_back(3); 
  v.push_back(2); 

  // ok
  std::for_each(v.begin(), v.end(), print); 
  std::cout << "--------------" << std::endl;

  // error, 由于 std::for_each() 要求的是仅接受一个参数的函数，所以不能直接传入 add() 函数
  //std::for_each(v.begin(), v.end(), add); 

  // Boost.Bind 简化了不同函数之间的绑定。 它只包含一个 boost::bind() 模板函数，定义于 boost/bind.hpp 中
  // 【注释1】
  std::for_each(v.begin(), v.end(), boost::bind(add, 10, _1)); 
  std::cout << "--------------" << std::endl;

  // ok, std::sort() 算法要求一个二元函数作为其第三个参数
  std::sort(v.begin(), v.end(), compare);
  print_vec(v);
  std::cout << "--------------" << std::endl;
  
  // ok, 由于 compare() 本身就是一个二元函数，所以使用 boost::bind() 确是多余的
  // 【注释2】
  std::sort(v.begin(), v.end(), boost::bind(compare, _1, _2));
  print_vec(v);
  std::cout << "--------------" << std::endl;

  // 不过使用 boost::bind() 还是有意义的。例如，如果容器要按升序排列而又不能修改 compare() 函数的定义
  // 【注释3】
  std::sort(v.begin(), v.end(), boost::bind(compare, _2, _1));
  print_vec(v);

  return 0;
} 
/*
g++ -o bind bind.cpp -I/Users/gerryyang/code/public/boost_1_54_0

mba:bind gerryyang$ ./bind 
1
3
2
--------------
11
13
12
--------------
3
2
1
--------------
3
2
1
--------------
1
2
3
 
* */
