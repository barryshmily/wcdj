#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define LOG(format, ...) \
	do{ \
		char __log_buff__[4096] = {0}; \
		snprintf(__log_buff__, sizeof(__log_buff__), "[pid=%d][%s %d]" format"\n", getpid(), __FILE__, __LINE__, ##__VA_ARGS__); \
		FILE* __fp=fopen("./tmp.log","a+"); \
		fwrite(__log_buff__, strlen(__log_buff__), 1 ,__fp ); \
		fclose(__fp); \
	} while(0)

int main()
{
	int a = 1;
	LOG("a[%d]", a);

	return 0;
}
// gcc -o tmp_log tmp_log.c 