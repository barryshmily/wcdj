#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define LOG(format, ...) \
	do{ \
		char __log_buff__[4096] = {0}; \
		snprintf(__log_buff__, sizeof(__log_buff__), "[%d][%d][%s:%s:%d]" format"\n", (int)time(NULL), getpid(), __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
		FILE* __fp=fopen("./tmp.log","a+"); \
		fwrite(__log_buff__, strlen(__log_buff__), 1 ,__fp ); \
		fclose(__fp); \
	} while(0)

#define LOG2(format, ...) \
	do { \
		if (g_log_switch) {\
			char __log_buff__[4096] = {0}; \
			char time[32] = {0}; \
			getstimeval(time); \
			snprintf(__log_buff__, sizeof(__log_buff__), "[%s][%d][%s:%s:%d]" format"\n", time, getpid(), __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
			FILE* __fp=fopen("./tmp.log","a+"); \
			fwrite(__log_buff__, strlen(__log_buff__), 1 ,__fp ); \
			fclose(__fp); \
		} \
	} while(0)

int g_log_switch;

static char * getstimeval(char *buf)
{
	struct timeval tv;
	struct tm      tm;
	size_t         len = 28;

	gettimeofday(&tv, NULL);
	localtime_r(&tv.tv_sec, &tm);
	strftime(buf, len, "%Y-%m-%d %H:%M:%S", &tm);
	len = strlen(buf);
	sprintf(buf + len, " %06.6d", (int)(tv.tv_usec));

	return buf;
}

int main()
{
	int a = 1;
	LOG("a[%d]", a);

	g_log_switch = 1;
	a = 2;
	LOG2("a[%d]", a);

	g_log_switch = 0;
	a = 3;
	LOG2("a[%d]", a);

	return 0;
}
// gcc -o tmp_log tmp_log.c 
