#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
//#define PROFILE

#if defined(PROFILE)
#include <gperftools/profiler.h>
#include <gperftools/heap-profiler.h>
#endif
#define MAX_TIME  10000

void mktimetest1()
{
	time_t t,t1;
	struct tm tm1,tm2;
	struct timeval begin,end;
	int i=0;
	t=time(NULL);
	localtime_r(&t,&tm1);
	gettimeofday(&begin,NULL);
	for(i=0;i<MAX_TIME;++i)
	{
		t1=mktime(&tm1);
		printf("t1=%ld\n",t1);
	}
	gettimeofday(&end,NULL);
	long mstime=end.tv_sec*1000000+end.tv_usec-begin.tv_sec*1000000-begin.tv_usec;
	printf("mktime Total msec=%fms \n" , mstime*1.0/1000);
}

void mktimetest2()
{
	time_t t,t1;
	struct timeval begin,end;
	int i=0;
	gettimeofday(&begin,NULL);
	for(i=0;i<MAX_TIME;++i)
	{
		struct tm tm2;
		tm2.tm_year=115;//1900+115
		tm2.tm_mon=10;
		tm2.tm_mday=12;
		tm2.tm_isdst=1;
		t1=mktime(&tm2);
		//printf("t1=%ld\n",t1);
	}
	gettimeofday(&end,NULL);
	long mstime=end.tv_sec*1000000+end.tv_usec-begin.tv_sec*1000000-begin.tv_usec;
	printf("mktime unset tm_isdst Total msec=%fms \n" , mstime*1.0/1000);
}

void mktimetest3()
{
	time_t t,t1;
	struct timeval begin,end;
	int i=0;
	char zone[]="CST";
	gettimeofday(&begin,NULL);
	for(i=0;i<MAX_TIME;++i)
	{
		struct tm tm2;
		tm2.tm_year=115;//1900+115
		tm2.tm_mon=10;
		tm2.tm_mday=12;
		tm2.tm_isdst=0;//almost same to -1
		t1=mktime(&tm2);
		//printf("t1=%ld\n",t1);
	}
	gettimeofday(&end,NULL);
	long mstime=end.tv_sec*1000000+end.tv_usec-begin.tv_sec*1000000-begin.tv_usec;
	printf("mktime set tm_isdst Total msec=%fms \n" , mstime*1.0/1000);
}

time_t testmktime()
{
	char * tz;
	tz = getenv ("TZ");
	printf("TZ:%s\n",tz);
	//mktimetest1();
	mktimetest2();
	mktimetest3();
}

int main()
{
#ifdef PROFILE
	ProfilerStart("test.prof");
#endif

	testmktime();
#ifdef PROFILE
	ProfilerStop();
#endif
	return 0;
}
