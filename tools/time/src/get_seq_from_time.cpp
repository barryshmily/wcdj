#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

// min + sec + (msec / 10)
unsigned int getSeq()
{
    struct timeval tvv;
    struct tm      tmm;
    char   tmp[20] = {0};
    time_t tt      = 0;

    gettimeofday(&tvv, NULL);
    tt = tvv.tv_sec;
    tmm = *localtime(&tt);

    sprintf(tmp, "%d%02d%05d", tmm.tm_min, tmm.tm_sec, (int)(tvv.tv_usec/10));

    return atoi(tmp);
}

inline void onIdle(int msec)
{
	struct timeval tval;
	if (msec < 1)
		msec = 1;

	tval.tv_sec  = msec / 1000;
	tval.tv_usec = (msec % 1000) * 1000;

	select(0, (fd_set*)0, (fd_set*)0, (fd_set*)0, &tval);
}


int main()
{
	for (int i = 0; i < 20; ++i)
	{
		printf("%u\n", getSeq());
		onIdle(1);
	}

	return 0;
}

/*
output:
123270638
123270818
123270933
123271056
123271176
123271298
123271420
123271531
123271642
123271754
123271868
123271994
123272104
123272211
123272319
123272441
123272549
123272669
123272783
123272907
 
 */
