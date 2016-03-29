#include <stdio.h>
#include <stdlib.h>
#include <climits>
#include <errno.h>

int main()
{
	char buf[] = "9223372036854775807";
	long long a;

	a = strtoll(buf, NULL, 10);
	if (a == 0LL
			|| (errno == ERANGE && (a == LLONG_MAX || a == LLONG_MIN))
	   ) {

		printf("error a[%lld] LLONG_MAX[%lld] LLONG_MIN[%lld]\n", a, LLONG_MAX, LLONG_MIN);
	} else {

		printf("%lld\n", a);
	}

	return 0;

}
