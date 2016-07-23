/*#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
void main()
{
	int *arr;// = { {4,5,7,9},{8,14,18,20},{10,16,24,29},{12,17,30,35} }
	int index=0, index1 = 3;
	int search = 11;
	arr = (int*)malloc(16*sizeof(int));
	for (int i = 0; i < 16; i++)
	{
		scanf("%d",&arr[i]);
	}
	while (index < 4 && index1 < 4&&index>=0&&index1>=0)
	{
		if (search == arr[index*4 + index1])
		{
			printf("found\n");
			break;
		}
		else if (search>arr[index*4 + index1])
		{
			index++;
		}
		else
		{
			index1--;
		}
	}
	_getch();
}*/