/*
构造函数的初始值什么情况下必不可少？成员初始化顺序和什么有关？
(1) 如果成员是const，引用，或者属于某种未提供默认构造函数的类类型，我们必须通过构造函数初始值列表为这些成员提供初值。
(2) 构造函数初始值列表只说明用于初始化成员的值，而不限定初始化的具体执行顺序。成员的初始化顺序与它们在类定义中的出现顺序一致：第一个成员先被初始化，然后第二个，以此类推。构造函数初始值列表中初始值的前后位置关系不会影响实际的初始化顺序。
解决方法：最好构造函数初始值的顺序与成员声明的顺序保持一致，而且如果可能的话，尽量避免使用某些成员初始化其他成员。
 */

#include <stdio.h>

class X {
	int i;
	int j;

public:
	X(int val): j(val), i(j) 
	{
	}
	
	int get_i()
	{
		return i;
	}

	int get_j()
	{
		return j;
	}
};

int main()
{
	X x(1);
	printf("x.i[%d] x.j[%d]\n", x.get_i(), x.get_j());

	return 0;
}
/*
gerryyang@mba:class$g++ init_class_obj.cpp -o init_class_obj
init_class_obj.cpp:8:24: warning: field 'j' is uninitialized when used here [-Wuninitialized]
        X(int val): j(val), i(j) 
                              ^
1 warning generated.
gerryyang@mba:class$ls
init_class_obj*  init_class_obj.cpp
gerryyang@mba:class$./init_class_obj 
x.i[32767] x.j[1] 
 */
