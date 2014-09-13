#include "A.h"
#include "B.h"

int main(int argc, _TCHAR* argv[])
{
	A a;
	a.funA();
	A::getTheApp().funA();

	B<int> b;
	b.funB();

	return 0;
}