/*#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<conio.h>
void calculate(int arr[],int len)
{
	int sum = 0,pos=0,end=0,max=0;
	for (int i = 0; i < len; i++)
	{
		sum = sum + arr[i];
		if (sum <= 0)
		{
			sum = 0;
			pos = i + 1;
		}
		if (max < sum) 
		{
			end = i;
			max = sum;
		}
	}
	printf("%d %d  %d",max,pos,end);
}
void main()
{
	int arr[] = {-2,-1,10,2,-8,-3,3,5,4};
	calculate(arr,8);
	_getch();
}*/