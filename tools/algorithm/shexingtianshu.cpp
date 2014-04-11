#include <cstdio>  
#include <cstring>  
#define MAXN 10  
int a[MAXN][MAXN];  

int main()  
{  
	int n, x, y, val=0;  
	scanf("%d",&n);  
	memset(a,0,sizeof(a));// clear array  
	val=a[x=0][y=n-1]=1;// set the first element  
	while (val<n*n)  
	{  
		while (x+1<n && !a[x+1][y])   a[++x][y]=++val;  
		while (y-1>=0 && !a[x][y-1])  a[x][--y]=++val;  
		while (x-1>=0 && !a[x-1][y])  a[--x][y]=++val;  
		while (y+1<n && !a[x][y+1])   a[x][++y]=++val;  
	}  
	for (x=0; x<n; ++x)  
	{  
		for (y=0; y<n; ++y)  
		{  
			printf("%3d",a[x][y]);  
		}  
		printf("\n");  
	}  
	return 0;  
}  
/* g++ -Wall -pipe -Os -o shexingtianshu shexingtianshu.cpp
 4
 10 11 12  1
  9 16 13  2
  8 15 14  3
  7  6  5  4
 */
