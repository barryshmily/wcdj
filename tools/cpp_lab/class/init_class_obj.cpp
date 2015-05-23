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
