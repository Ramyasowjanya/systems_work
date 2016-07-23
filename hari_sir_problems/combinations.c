/*#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<conio.h>
#include<math.h>
void computeCombinations(char str[],int len)
{
	int limit = pow(2, len),temp;
	for (int index = 0; index < limit; index++)
	{
		temp = index;
		for (int index1 = 0; index1 < len,temp>0; index1++)
		{
			if (temp&1)
				printf("%c",str[index1]);
			temp = temp >> 1;
		}
		printf("\n");
	}
	_getch();
}
void main()
{
	char str[20];
	scanf("%s",str);
	computeCombinations(str,strlen(str));
}*/