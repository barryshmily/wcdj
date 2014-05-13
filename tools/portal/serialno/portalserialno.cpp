
void make_rand_portalserialno()
{
	// Ëæ»úÁ÷Ë®ºÅ
	char *szServiceCode = "Mobile_Test";
	char szLocalTime[36] = {0};
	time_t now = time(NULL);
	struct timeval now2;
	gettimeofday(&now2, NULL);
	strftime(szLocalTime, sizeof(szLocalTime), "%Y%m%d-%H%M%S", localtime(&now));
	snprintf(szLocalTime + strlen(szLocalTime), sizeof(szLocalTime) - strlen(szLocalTime), "%lu", now2.tv_usec);
	char szPsn[64] = {0};
	static unsigned uCount = 1;
	snprintf(szPsn, sizeof(szPsn), "%s-%.18s%d", szServiceCode, szLocalTime, uCount++%10);
	mapReqPara["PortalSerialNo"] = szPsn;
}

