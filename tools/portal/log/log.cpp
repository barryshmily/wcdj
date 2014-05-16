
#define __log(format, ...) \
	do{ \
		char __log_buff__[8192] = {0}; \
		snprintf(__log_buff__, sizeof(__log_buff__), "[%s %d]" format, __FILE__, __LINE__, ##__VA_ARGS__); \
		FILE* __fp=fopen("/tmp/gerry.log","a+"); \
		fwrite(__log_buff__, strlen(__log_buff__), 1 ,__fp ); \
		fclose(__fp); \
	} while(0)


