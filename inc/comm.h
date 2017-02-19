#ifndef __COMM_H__
#define __COMM_H__

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

#define E_FAIL -1
#define E_OK 0

// print log to tmp file
#define __LOG(format, ...) \
	do { \
		char __log_buff__[8192] = {0}; \
		snprintf(__log_buff__, sizeof(__log_buff__), "[pid=%d,%s:%d] "format, getpid(), __FILE__, __LINE__, ##__VA_ARGS__); \
		FILE* __fp = fopen("/tmp/gerry.log","a+"); \
		fwrite(__log_buff__, strlen(__log_buff__), 1 ,__fp ); \
		fclose(__fp); \
	} while (0)

// print log to terminal or file
#define INFO(format, ...) \
	do { \
		printf("INFO: [pid=%d,%s:%d]"format, getpid(), __FILE__, __LINE__, ## __VA_ARGS__); \
	} while (0)

#define ERROR(format, ...) \
	do { \
		printf("ERROR: [pid=%d,%s:%d]"format, getpid(), __FILE__, __LINE__, ## __VA_ARGS__); \
	} while (0)

#endif
