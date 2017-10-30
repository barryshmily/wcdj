#include <iostream>

class Base {
public:
	virtual void f1() = 0;
	virtual void f1(int) {
		std::cout << "virtual void Base::f1(int)\n";
	}
	virtual void f2() {
		std::cout << "virtual void Base::f2()\n";
	}
	void f3();
    
};

class Derived: public Base {
public:

	// 让Base class内名为f1的函数在Derived class作用域内可见，如果不这样声明，下面d.f1(1)会找不到
	using Base::f1;

    virtual void f1() {
		std::cout << "virtual void Derived::f1()\n";
	}
	void f3() {
		std::cout << "void Derived::f3()\n";
	}
};

int main()
{
	Derived d;

	d.f1();
	d.f1(1); // error ?
	d.f2();
	d.f3();

	return 0;
}
/*
virtual void Derived::f1()
virtual void Base::f1(int)
virtual void Base::f2()
void Derived::f3()
 */
