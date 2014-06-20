#if 0
二分查找算法

在计算机科学中，折半搜索，也称二分查找算法、二分搜索，是一种在有序数组中查找某一特定元素的搜索算法。搜素过程从数组的中间元素开始，如果中间元素正好是要查找的元素，则搜素过程结束；如果某一特定元素大于或者小于中间元素，则在数组大于或小于中间元素的那一半中查找，而且跟开始一样从中间元素开始比较。如果在某一步骤数组为空，则代表找不到。这种搜索算法每一次比较都使搜索范围缩小一半。

#endif

#include <stdio.h>

void bubblesort(int a[], int n)
{
	// select min item to first place
	int tmp = 0;

	for (int i = 1; i < n; ++i)
	{
		for (int j = n - 1; j > i - 1; --j)
		{
			if (a[j-1] > a[j])
			{
				tmp    =  a[j];
				a[j]   =  a[j-1];
				a[j-1] =  tmp;
			}
		}

	}
}

void printIntArray(int a[], int size)
{
	for (int i = 0; i != size; ++i)
	{
		printf("%d ", a[i]);
	}
	printf("\n");
}

int bsearch(int a[], int size, int key)
{
	int mid = 0;
	int left = 0;
	int right = size - 1;

	while (left <= right)
	{
		mid = (left + right) / 2;
		if (a[mid] == key)
		{
			return mid;
		} 
		else if (a[mid] > key)
		{
			right = mid - 1;
		}
		else if (a[mid] < key)
		{
			left = mid + 1;
		}
	}

	return -1;
}

int main(int argc, char **argv)
{
	int a[] = {2, 1, 4, 6, 3, 5, 7};
	int len = sizeof(a)/sizeof(int);
	printIntArray(a, len);

	bubblesort(a, len);
	printIntArray(a, len);

	printf("bsearch(a, len, 12) = %d\n", bsearch(a, len, 12));
	printf("bsearch(a, len, 5) = %d\n",  bsearch(a, len, 5));
	printf("bsearch(a, len, 2) = %d\n",  bsearch(a, len, 2));

	return 0;
}
/*
output:
2 1 4 6 3 5 7 
1 2 3 4 5 6 7 
bsearch(a, len, 12) = -1
bsearch(a, len, 5) = 4
bsearch(a, len, 2) = 1
 */
