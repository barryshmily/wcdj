

inline void onIdle(int msec)
{
	struct timeval tval;
	if (msec < 1)
		msec = 1;

	tval.tv_sec  = msec / 1000;
	tval.tv_usec = (msec % 1000) * 1000;

	select(0, (fd_set*)0, (fd_set*)0, (fd_set*)0, &tval);
}

