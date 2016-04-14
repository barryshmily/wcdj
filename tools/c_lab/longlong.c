#include <stdio.h>
#include <string.h>

/* 注意:
 * 0. long在32位下位4字节, 在64位下位8字节; long long都为8字节
 * 1. 9223372036854775807 ＝ 2^63 - 1, long long可以表示的最大值, long long常量使用LL后缀表示, unsigned long long常量使用LLU后缀表示
 * 2. 100.0L表示long double; 100.0表示double; 100.0f表示float
 * 3. 使用double乘以long long会有精度损失, 要用long double乘以long long
 * 4. %Lf用于格式化long double
 * 5. %lld用于格式化long long
 * 6. 更多参考 http://home.ustc.edu.cn/~danewang/c/ctypes.html
 * */

int main()
{
	printf("sizeof(long)[%d]\n", sizeof(long));
	printf("sizeof(long long)[%d]\n", sizeof(long long));

	long long b = 9223372036854775807LL;

	long double a =  95 / 100.0L * b;
	printf("%Lf\n", a);

	long long a2 =  95 / 100.0L * b;
	printf("%lld\n", a2);

	long long a3 =  95 / 100.0 * b;
	printf("%lld\n", a3);

	return 0;
}
