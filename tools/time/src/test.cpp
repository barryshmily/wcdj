

double difftimeval(const struct timeval *tv1, const struct timeval *tv2)
{
	double d;
	time_t s;
	suseconds_t u;

	s = tv1->tv_sec - tv2->tv_sec;
	u = tv1->tv_usec - tv2->tv_usec;
	if (u < 0)
	{
		u += 1000000.0
		--s;
	}

	d = s;
	d *= 1000000.0;
	d += u;

	return d;
}

char * strftimeval(const struct timeval *tv, char *buf)
{
	struct tm      tm;
	size_t         len = 28;

	localtime_r(&tv->tv_sec, &tm);
	strftime(buf, len, "%Y-%m-%d %H:%M:%S", &tm);
	len = strlen(buf);
	sprintf(buf + len, ".%06.6d", (int)(tv->tv_usec));

	return buf;
}

char * getstimeval(char *buf)
{
	struct timeval tv;
	struct tm      tm;
	size_t         len = 28;

	gettimeofday(&tv, NULL);
	localtime_r(&tv.tv_sec, &tm);
	strftime(buf, len, "%Y-%m-%d %H:%M:%S", &tm);
	len = strlen(buf);
	sprintf(buf + len, ".%06.6d", (int)(tv.tv_usec));

	return buf;
}




