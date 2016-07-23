/*#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
int findNonRepeatno(int arr[],int count[],int index)
{
	for (int i = 0; i < index; i++)
	{
		if (count[i] == 1)
			return arr[i];
	}
	return -1;
}
void insert(int data,int arr[],int count[],int *index)
{
	int index1;
	for (index1 = 0; index1 < *index; index1++)
	{
		if (data == arr[index1])
		{
			count[index1] += 1;
			break;
		}
	}
	if (index1 == *index)
	{
		arr[*index] = data;
		count[(*index)] = 1;
		(*index)++;
	}
}
void findNonrepeating()
{
	int choice = 0,input=0;
	int arr[100], count[100],index=0,nonrepeat;
	for (int i = 0; i < 100; i++)
		count[i] = 0;
	while (choice < 3)
	{
		printf("1.number insert\n");
		printf("2.number check\n");
		scanf("%d",&choice);
		switch (choice)
		{
		case 1:if (input != -1)
				{
					scanf("%d", &input);	
					insert(input, arr, count, &index);
				}
				break;
		case 2:nonrepeat = findNonRepeatno(arr, count, index);
			printf("non repeating is %d",nonrepeat);
			break;
		case 3:break;
		}
	}
}
void main()
{
	findNonrepeating();
}*/