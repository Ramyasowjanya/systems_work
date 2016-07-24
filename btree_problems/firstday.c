#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
struct students
{
	int type;
	int rollno;
	char name[28];
	char college[28];
};
struct nonleafpage
{
	int type;
	int offset[3];
	int keys[2];
	char unused[40];
};
int lengthOfString(char *name)
{
	int len = 0, i = 0;
	while (name[i] != '\0')
	{
		++i;
		++len;
	}
	return len;
}
int string_compare(char *str1, char *str2)
{
	int len1,len2;
	len1 = lengthOfString(str1);
	len2 = lengthOfString(str2);
	if (len1 == len2)
	{
		while (len1 > 0)
		{
			if (str1[len1 - 1] != str2[len1 - 1]&& (str1[len1 - 1] != str2[len1 - 1]-32)&& (str1[len1 - 1]-32 != str2[len1 - 1]))
			{
				return 0;
			}
			len1--;
		}
		return 1;
	}
	return 0;
}
void strcopy(char *src, char *dest, int end, int start)
{
	int i = 0;
	for (i = start; i < end; i++)
		dest[i - start] = src[i];
	dest[i - start] = '\0';
}
void insertIntoList(struct students *list,int rollno,char *name,char *clg,int nlen,int clen)
{
	list->rollno = rollno;
	strcopy(name,list->name,nlen,0);
	strcopy(clg, list->college, clen, 0);
}
void main()
{
	FILE *fp,*fp1;
	char ch; 
	int i = 0, nonleafc = 0, studcount = 0, temp = 1, rollno,par,parr;
	int *arr=&par,*arr1=&parr,rem=0,inputroll1;
	int level=0,index = 0, offset = 0, indexn = 0, flag = 0, indexc = 0, inputroll;
	char clg[28];
	struct students data;
	struct nonleafpage nonleaf;
	fp = fopen("data.csv", "r");
	fp1 = fopen("studentsfilebin.bin", "wb+");
	while (!feof(fp))
	{
		ch = fgetc(fp);
		memset(&data, 0, sizeof(data));
		flag = 0; indexn = 0; indexc = 0; data.rollno = 0;
		while (ch != '\n'&&!feof(fp))
		{
			if (ch != ','&&flag == 0)
			{
				data.rollno= (data.rollno* 10) + ch - '0';
			}
			else if (ch == ','&&flag == 0)
			{
				flag++;
			}
			else if (ch != ','&&flag == 1)
			{
				data.name[indexn++] = ch;
			}
			else if (ch == ','&&flag == 1)
			{
				flag++;
			}
			else if (ch != '\n'&&flag == 2)
			{
				data.college[indexc++] = ch;
			}
			ch = fgetc(fp);
		}
		if (flag == 2)
		{
			data.college[indexc] = '\0';
			data.name[indexn] = '\0';
			data.type = 0;
			fwrite(&data, 64, 1, fp1);
			studcount++;
		}
	}
	fclose(fp);
	printf("Enter rollno1 and rollno2 to find count of students in that range");
	scanf("%d%d",&inputroll,&inputroll1);
	scanf("%s",clg);
	flag = 0; indexc = 0; indexn = studcount/3;
	offset = studcount*64;
		while (indexn > 0)
		{
			flag = 0;
			while (flag < indexn)
			{
				index = 0; //indexn++;
				memset(&nonleaf, 0, sizeof(nonleaf));
				nonleaf.offset[index++] = indexc++;
					nonleaf.offset[index++] = indexc++;
					nonleaf.offset[index++] = indexc++;
				index = 1;
				level = temp;
				fseek(fp1, (indexc - 2) * 64, SEEK_SET);
					while (level > 0)
					{
						fread(arr, sizeof(arr), 1, fp1);
						fread(arr1, sizeof(arr1), 1, fp1);
						level--;
						fseek(fp1, ((*arr1)) * 64, SEEK_SET);
					}
					nonleaf.keys[0] = *arr1;		
				level = temp;
					fseek(fp1, (indexc - 1) * 64, SEEK_SET);
					while (level > 0)
					{
						fread(arr, sizeof(arr), 1, fp1);
						fread(arr1, sizeof(arr1), 1, fp1);
						level--;
						fseek(fp1, ((*arr1)) * 64, SEEK_SET);
					}
					nonleaf.keys[1] = *arr1;
				flag++;
				nonleaf.type = 1;
				fseek(fp1, (studcount + nonleafc) * 64, SEEK_SET);
				fwrite(&nonleaf, sizeof(nonleaf), 1, fp1);
				nonleafc++;
			}
			temp++;
			indexn = (indexn / 3);
		}	
	fclose(fp1);
	fp1 = fopen("studentsfilebin.bin", "rb");
	fseek(fp, -64, SEEK_END); offset = -1;
	fread(&nonleaf, sizeof(nonleaf), 1, fp1);
	flag = 0;
	while (temp > 1)
	{
		flag = 0;
		for (i = 0; i < 2; i++)
		{
			if (nonleaf.keys[i] <= inputroll)
			{
				if ((i != 1 && nonleaf.keys[i + 1]>inputroll)||i==1)
				{
					flag = 1;
					offset = (nonleaf.offset[i + 1]) * 64;
					break;
				}
			}
			else if (nonleaf.keys[i] > inputroll)
			{
				flag = 1;
				offset = (nonleaf.offset[i]) * 64;
				break;
			}
		}
		if (flag==0&& nonleaf.keys[i] >= inputroll)
			offset = (nonleaf.offset[i + 1]) * 64;
		temp--;
		fseek(fp1, offset, SEEK_SET);
		fread(&nonleaf, sizeof(nonleaf), 1, fp1);
	}
	i = 0;
	fseek(fp1, offset, SEEK_SET);
	fread(&data, sizeof(data), 1, fp1);
	if (data.rollno >= inputroll&&string_compare(data.college,clg))
	{
		printf("%s - ", data.name);
		printf("%s\n ", data.college);
		i++;
	}
	do {
		fread(&data, sizeof(data), 1, fp1);
		printf("%d -- ",data.rollno);
		if (data.type == 0 && string_compare(data.college, clg))
		{
			i++; printf("%s - ", data.name);
			printf("%s\n ", data.college);
			break;
		}
	} while (data.rollno < inputroll1&&data.type==0);
	printf("count of students is %d",i);
	fclose(fp1);
	_getch();
}

