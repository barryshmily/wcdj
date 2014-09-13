// B.h
#ifndef _B_H_
#define _B_H_

// 定义模板类B
template <typename T>
class B
{
public:
	B(){}
	B(T tvar, int ivar = 0):// 初始化
	  m_tb(tvar), m_ib(ivar){}
	void funB();

	T m_tb;
	int m_ib;
};

template <typename T>
void B<T>::funB()
{
	printf("funB...\n");
	A::getTheApp().funA();

	A a;
	printf("a.m_iBa.m_ib = %d\n", a.m_iBa.m_ib);// 疑问，m_ib为什么没有被初始化为0
}

/* 说明：
 * (1) 在B.h类模版中使用到了A.h中的定义，但是在B.h中不需要知道A的完整定义。
 * 即，模版类B中可以使用任何类型，当且仅当对模版类B实例化时，才必须包含A类型的定义。
 * 
 * (2) 将 #include "A.h" 放在模板类B定义之后，原因是，在A.h中需要事先知道模板类B的定义。
*/
#include "A.h"

#endif