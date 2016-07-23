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
	int count = 0;
	if (index < len)
	{
		swap(&str[index + 1], &str[index]);
		printf("%s\n", str);
		permute(str, index + 1, len);
		swap(&str[index + 1], &str[index]);
		// printf("%s\n", str);
	}
}
void main()
{
	char str[7] ;
	scanf("%s",str);
	permute(str,1,5);
	_getch();
}*/
/*
#define _CRT_SECURE_NO_WARNINGS
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
void permute(char *str,int open,int close,int len,int position)
{
	if (close == len)
	{
		printf("%s\n",str);
	}
	else
	{
		if (open > close)
		{
			str[position] = ')';
			permute(str, open, close+1, len, position+1);
		}
		if (open < len)
		{
			str[position] = '(';
			permute(str, open+1, close, len, position + 1);
		}
	}
}
void main()
{
	char *str;
	int len;
	scanf("%d",&len);
	permute(str,0,0,len,0);
	_getch();
}*/