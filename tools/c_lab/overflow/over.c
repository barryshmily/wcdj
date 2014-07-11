#include <stdio.h>
#include <stdlib.h>
//#include <iostream>
//using namespace std;

int calcPayAmt(const int coins)
{
	int iPayAmt = (coins * 100) / 10;
	printf("calc: iPayAmt[%d]\n", iPayAmt);
	//cout << typeid(coins * 100).name() << endl;

	return iPayAmt;

}

int main()
{
	//int j = 2147483647;
	//printf("%d\n", atoi("21474836480"));

	int iPayAmt = (1932735278 * 100) / 10;
	printf("iPayAmt[%d]\n", iPayAmt);
	//cout << typeid(1932735278 * 100).name() << endl;

	//iPayAmt = (193273527800) / 10;
	//printf("iPayAmt[%d]\n", iPayAmt);
	//cout << typeid(193273527800).name() << endl;

	const int tmp = 1932735278;
	iPayAmt = (tmp * 100) / 10;
	printf("iPayAmt[%d]\n", iPayAmt);
	//cout << typeid(tmp * 100).name() << endl;

	int coins = 1932735278;
	iPayAmt = calcPayAmt(coins);
	printf("iPayAmt[%d]\n", iPayAmt);

#if 0
	while (0)
	{
		int k = 1932735278; 

		int i = (k * 100) % 10;
		if (i == 0)
		{
			printf("i=%d, j=%lu\n",i, j);

			int p = (k * 100) / 10;
			if (p > 0)
			{
				printf("--> p=%d, k=%d\n", p, k);
				//break;
				sleep(1);
			}
			else
			{
				printf("==> p=%d, k=%d\n", p, k);
				;
			}

			//sleep(1);

		}
		j--;	

	}

#endif

	return 0;
}
/*
output:
iPayAmt[-52]
iPayAmt[-52]
calc: iPayAmt[-52]
iPayAmt[-52]
 * */

