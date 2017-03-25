#include <stdio.h>
#include <iostream>
using namespace std;

class base {
public:
	base() {cout << "base()\n";}
	~base() {cout << "~base()\n";} // note, have no virtual

private:
	int v1;
};

class derived : public base {
public:
	derived() {cout << "derived()\n";}
	~derived() {cout << "~derived()\n";}

private:
	int v2;
};

int main() 
{
	//derived obj;

	base *b = new derived;
	// do something
	delete b;

	return 0;
}
/*
output:
base()
derived()
~base()
 */
