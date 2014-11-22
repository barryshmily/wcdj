#include <stdio.h>

#define COLORFUL_BEGIN  "\033[0;31;40m"
#define COLORFUL_END    "\033[0m"

int main()
{
	printf("%s begin\n", COLORFUL_BEGIN);
	printf("%s end\n", COLORFUL_END);

	return 0;
}
