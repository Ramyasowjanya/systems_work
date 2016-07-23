/*#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<conio.h>
void quicksort(int arr[],int low,int high)
{
	int p;
	if (low < high)
	{
		p = partition(arr, low, high);
		quicksort(arr, low, p-1);
		quicksort(arr, p + 1, high);
	}
}
void swap(int *a,int *b)
{
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}
int partition(int arr[], int low, int high)
{
	int pivot, i;
	pivot =arr[high];
	i = low;        // place for swapping
	for (int j = low; j < high; j++)
	{
		if (arr[j] <= pivot)
		{
			swap(&arr[i],&arr[j]);
			i= i + 1;
		}
	}
	swap (&arr[i],&arr[high]);
	return i;
}
void main()
{
	int arr[] = { 3,6,1,2,8,4,5 };
	quicksort(arr,0,6);
	for (int i = 0; i < 7; i++)
		printf("%d ",arr[i]);
	_getch();
}*/