#include <stdio.h>
#include <iostream>
using namespace std;

int main()
{
#if 0
	int icoins = 1000000000;
	int ipayamt = (icoins * 100) / 10;

	printf("payamt[%d]\n", ipayamt);
#endif

	int a = 2147483647 + 1;
	printf("int a = 2147483647 + 1 = [%d]\n", a);

	unsigned int b = 2147483647 + 1;
	printf("unsigned int b = 2147483647 + 1 = [%u]\n", b);

	int c = 2147483647 * 10;
	printf("int c = 2147483647 * 10 = [%d]\n", c);

	unsigned long d = 2147483647 * 10;
	printf("unsigned long d = 2147483647 * 10 = [%lu]\n", d);

	unsigned long e = 2147483647 * 10UL;
	printf("unsigned long e = 2147483647 * 10UL = [%lu]\n", e);

	unsigned long f = (long)2147483647 * 10;
	printf("unsigned long f = (long)2147483647 * 10 = [%lu]\n", f);

	long g = 2147483647 * 10;
	printf("long g = 2147483647 * 10 = [%lu]\n", g);

	printf("sizeof(unsigned long)=%d\n", sizeof(unsigned long));

	cout << typeid(2147483647 * 10).name() << endl;
	cout << typeid(2147483647 * 10UL).name() << endl;
	cout << typeid(0.0f).name() << endl;
	cout << typeid(1).name() << endl;
	cout << typeid(a).name() << endl;
	cout << typeid(b).name() << endl;
	cout << typeid(c).name() << endl;
	cout << typeid(d).name() << endl;
	cout << typeid(e).name() << endl;
	cout << typeid(f).name() << endl;
	cout << typeid(g).name() << endl;

	return 0;
}
/*
测试发生overflow时可能出现的情况
待验证：常量int在计算发生overflow提升为long或unsigned long时，前面字节为什么填充0？
gerryyang@mba:overflow$echo "ibase=16;FFFFFFFFFFFFFFF6" | bc
18446744073709551606
 * */
