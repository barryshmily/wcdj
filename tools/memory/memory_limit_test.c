#include<stdio.h>  
#include<malloc.h>  
#include <string.h>
int main()  
{  
	char *p = NULL;  
	const int bs = 10 * 1024 * 1024;

	for (;;) {
		p = (char *)malloc(bs);  
		if (p) {  
			sleep(1);
			memset(p, 0x1, bs);
			//printf("Memory allocated at: %x/n", p); 
		} else {  
			printf("Not enough memory!/n");
		}
	}

	return 0;  
}