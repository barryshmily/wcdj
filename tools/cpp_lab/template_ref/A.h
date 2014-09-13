// A.h
#ifndef _A_H_
#define _A_H_

#include "B.h"// A要知道模板类B的定义

class A
{
public:
	static A& getTheApp();
	void funA();
	typedef B<int> BINT;
	BINT m_iBa;// 模板类B类型的成员

private:
	static A* theApp;	

};
#endif