#include <stdio.h>

class Person;
void func(Person &p)
{
	printf("func\n");
}

int main()
{
	printf("main\n");
	return 0;
}

// 定义func函数，但不需要Person的定义。但是，在调用func函数时，就需要知道Person的定义。也就是，比如一个函数库有非常多的函数，但是我们可能只用到了其中很少的函数，对我们用到的函数，在客户端通过前置声明的方式（而不是包含所有定义的方式），可以减少对不必要类型定义的依赖。
