/*#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
void merge(int *arr1,int start,int mid,int end)
{
	int *arr3 = (int *)malloc(sizeof(int)*5);
	int index=start, index1=mid+1,index3=start;
	while (index <=mid&&index1 <=end)
	{
		if (arr1[index] < arr1[index1])
		{
			arr3[index3++] = arr1[index];
			index++;
		}
		else if (arr1[index] > arr1[index1])
		{
			arr3[index3++] = arr1[index1];
			index1++;
		}
		else
		{
			arr3[index3++] = arr1[index1];
			index1++; index++;
		}
	}
	while (index <=mid)
		{
			arr3[index3++] = arr1[index];
			index++;
		}
	while (index1 <= end)
		{
			arr3[index3++] = arr1[index1];
			index1++;
		}

	for (index1 = start; index1 <index3; index1++)
	{
		arr1[index1] = arr3[index1];
	}
}
void mergesort(int *arr1,int min,int max)
{
	if (min < max)
	{
		int mid;
		mid =( min + max )/ 2;
		mergesort(arr1, min,mid);
		mergesort(arr1, mid+1,max );
		merge(arr1, min,mid,max);
	}
}
void main()
{
	int arr1[] = {-1,4,6,3,9}, *arr2, len1 = 5, len2 = 6, index, index1 = 0, *arr3, index3 = 0;
	//scanf("%d %d",&len1,&len2);
	//arr1 = (int*)malloc(len1*sizeof(int));
	//arr2 = (int*)malloc(len2*sizeof(int));
	//arr3 = (int*)malloc((len2+len1)*sizeof(int));
	/*for (index = 0; index < len1; index++)
		scanf("%d",&arr1[index]);
		//arr1[index] = index + 1;
	mergesort(arr1,0,len1-1);
	printf("\n");
	for (index = 0; index < len1; index++)
		printf("%d ",arr1[index]);
	_getch();
}*/