/*#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
void swap(char *a, char *b)
{
	char te;
	te = *a;
	*a = *b;
	*b = te;
}
void permute(char *str,int index,int len)
{
	if (index == len-1)
	{
		printf("%s\n",str);
	}
	for (int i = index; i < len; i++)
	{
		swap(&str[i],&str[index]);
		permute(str,index+1,len);
		swap(&str[i], &str[index]);
	}
}
void main()
{
	char str[4] ;
	scanf("%s",str);
	permute(str,0,4);
	_getch();
}*/