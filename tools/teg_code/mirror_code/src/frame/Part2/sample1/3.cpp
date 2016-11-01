#include<cstdio>
#include<algorithm>
#include<cstdlib>
#include<map>
#include<vector>
#include<set>
#include<cstring>
#include<functional>
#include<string>
#include<iostream>
#include<queue>

using namespace std;

int v[500001];

int main()
{
	int n,i,t=0,l,r,m,j;
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		scanf("%d",v+i);
	}
	sort(v,v+n);
	l=0;
	r=n/2;
	t=0;
	while(l<=r)
	{
		m=(l+r)>>1;
		i=m-1;j=n-1;
		bool can =true;
		while(i>=0)
		{
			if(v[j]<2*v[i])
			{
				can=false;
				break;
			}
			i--;
			j--;
		}
		if(can)
		{
			l=m+1;
			t=m;
		}
		else
		{
			r=m-1;
		}
	}
	printf("%d\n",n-t);
	return 0;
}

