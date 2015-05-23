/*
 delegating constructors are permitted only in C++11
 */

#include <stdio.h>

class X {
	int i;
	int j;

public:
	// delegating constructor
	X(int a, int b): i(a), j(b)
	{
		printf("call X(int a, int b)\n");
	}

	X(): X(1, 2) 
	{
		printf("call X()\n");
	}

	X(int a): X(a, 0) 
	{
		printf("call X(int a)\n");
	}

};

int main()
{
	X x1;
	printf("--------------\n");
	X x2(100);
	printf("--------------\n");
	X x3(3, 4);

	return 0;
}
/*
gerryyang@mba:c++11$g++ -o delegating_constructor delegating_constructor.cpp 
delegating_constructor.cpp:18:7: error: delegating constructors are permitted only in C++11
        X(): X(1, 2) 
             ^
delegating_constructor.cpp:23:12: error: delegating constructors are permitted only in C++11
        X(int a): X(a, 0) 
                  ^
2 errors generated.
gerryyang@mba:c++11$g++ -std=c++11 -o delegating_constructor delegating_constructor.cpp 
gerryyang@mba:c++11$./delegating_constructor 
call X(int a, int b)
call X()
--------------
call X(int a, int b)
call X(int a)
--------------
call X(int a, int b) 
 */
