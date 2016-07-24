/*#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
struct teachingstaff
{
	int staffid;
	char name[24];
	char college[24];
	char dept[12];
};
struct leafpagestaff
{
	int pagetype;
	int tableid;
	struct teachingstaff table[7];
	char unused[56];
};
struct students
{
	int rollno;
	char name[24];
	char college[20];
};
struct nonleafpage
{
	int type;
	int offset[64];
	int keys[63];
};
struct leafpagestud
{
	int type;
	int tableid;
	struct students data[10];
	char unused[24];
};
struct tablesector
{
	int tableid[8];
	int rootpageid[8];
};
void main()
{
	FILE *fp, *fp1,*fp2;
	char ch;
	int count = 0,tablecount=0, flag = 0, index = 0,tcount=0,*tableid,temp=1;
	int offsetc=0,indexn = 0, indexc = 0, type = 1, ncount = 0,indexd=0;
	int offsets = 0, scount = 0,rollno,staffid;
	char sunused[24] = { ' ' }, tunused[56] = {' '};
	struct students data;
	int sid,tid;
	struct nonleafpage snonleaf,tnonleaf;
	struct leafpagestud leaf;
	struct leafpagestaff staffleaf;
	struct tablesector table;
	struct teachingstaff staff;
	fp = fopen("data.csv", "r");
	fp1 = fopen("studentsfilelakh1.bin", "wb+");
	fp2 = fopen("emp.csv","r");
	fseek(fp1, 64, SEEK_SET);
	while (!feof(fp) && count < 640&&!feof(fp2) && tablecount < 448)
	{
		index = 0; type = 0;
		//memset(&leaf, 0, sizeof(leaf));
		fwrite(&type, sizeof(type), 1, fp1);
		temp = 1;
		tableid = &temp;
		fwrite(&tableid, sizeof(tableid), 1, fp1);
		while (index < 10)
		{
			ch = fgetc(fp);
			memset(&data, 0, sizeof(data));
			flag = 0; indexn = 0; indexc = 0;
			while (ch != '\n'&&!feof(fp))
			{
				if (ch != ','&&flag == 0)
				{
					data.rollno = (data.rollno * 10) + ch - '0';
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
				fwrite(&data, sizeof(data), 1, fp1);
				count++;
			}
			if (index == 0)
				rollno=data.rollno;
			index++;
		}
		if (flag == 2)
		{
			fwrite(&sunused, sizeof(sunused), 1, fp1);
			tcount++;
			if (count > 10)
			{
				if (count == 20)
				{
					snonleaf.offset[offsetc++] = 0;
				}
				snonleaf.offset[offsetc++] = tcount - 1;
				snonleaf.keys[ncount++] = rollno;
				snonleaf.type = 1;
				tcount++;
				fwrite(&snonleaf, sizeof(snonleaf), 1, fp1);
				fseek(fp1, 0, SEEK_SET);
				table.rootpageid[0] = tcount-1;
				fwrite(&table, sizeof(table), 1, fp1);
				fseek(fp1, 64+(tcount*512), SEEK_SET);
			}
		}
		index = 0; type = 0;
		fwrite(&type, sizeof(type), 1, fp1); 
		temp = 2;
		tableid = &temp;
		fwrite(&tableid, sizeof(tableid), 1, fp1);
		while (index < 7)
		{
			ch = fgetc(fp2);
			memset(&staff, 0, sizeof(staff));
			flag = 0; indexn = 0; indexc = 0; indexd = 0; staff.staffid = 0;
			while (ch != '\n'&&!feof(fp2))
			{
				if (ch != ','&&flag == 0)
				{
					staff.staffid = (staff.staffid * 10) + ch - '0';
				}
				else if (ch == ','&&flag == 0)
				{
					flag++;
				}
				else if (ch != ','&&flag == 1)
				{
					staff.name[indexn++] = ch;
				}
				else if (ch == ','&&flag == 1)
				{
					flag++;
				}
				else if (ch != ','&&flag == 2)
				{
					staff.college[indexc++] = ch;
				}
				else if (ch == ','&&flag == 2)
				{
					flag++;
				}
				else if (ch != '\n'&&flag == 3)
				{
					staff.dept[indexd++] = ch;
				}
				ch = fgetc(fp2);
			}
			if (flag == 3)
			{
				staff.college[indexc] = '\0';
				staff.name[indexn] = '\0';
				staff.dept[indexd] = '\0';
				//insertIntoList(&data, i, name, clg, indexn + 1, indexc + 1);
					//printf("%d - ", data.rollno);
					//printf("%s - ", data.name);
					//printf("%s \n", data.college);
				fwrite(&staff, sizeof(staff), 1, fp1);
				tablecount++;
			}
			if (index == 0)
				staffid = staff.staffid;
			index++;
		}
		if (flag == 3)
		{
			fwrite(&tunused, sizeof(tunused), 1, fp1);
			tcount++;
			if (tablecount >7)
			{
				if (tablecount == 14)
				{
					tnonleaf.offset[offsets++] = 1;
				}
				tnonleaf.offset[offsets++] = tcount - 1;
				tnonleaf.keys[scount++] = staffid;
				tcount++;
				tnonleaf.type = 1;
				fwrite(&tnonleaf, sizeof(tnonleaf), 1, fp1);
				fseek(fp1, 0, SEEK_SET);
				table.rootpageid[1] = tcount - 1;
				fwrite(&table, sizeof(table), 1, fp1);
				fseek(fp1, 64 + (tcount * 512), SEEK_SET);
			}
		}
	}
	fclose(fp1);
	scanf("%d%d",&sid,&tid);
	fp1 = fopen("studentsfilelakh1.bin", "rb");
	memset(&table,0,sizeof(table));
	fread(&table,sizeof(table),1,fp1);
	fseek(fp1,(table.rootpageid[0]*512),SEEK_CUR);
	fread(&snonleaf, sizeof(snonleaf), 1, fp1);
	int offset;
	for (int i = 0; i < 63; i++)
	{
		if (snonleaf.keys[i] <= sid)
		{
			if ((i != 62 && snonleaf.keys[i + 1]>sid) || i == 62)
			{
				flag = 1;
				offset =64+( (snonleaf.offset[i + 1]) * 512);
				break;
			}
		}
		else if (snonleaf.keys[i] > sid)
		{
			flag = 1;
			offset = 64 + ((snonleaf.offset[i]) * 512);
			break;
		}
	}
	flag = 0;
	fseek(fp1,offset,SEEK_SET);
	fread(&leaf, sizeof(leaf), 1, fp1);
	for (int i = 0; i < 10; i++)
	{
		//printf("%d\n",leaf.data[i].rollno);
		if (leaf.data[i].rollno == sid)
		{
			flag = 1;
			printf("%s - ", leaf.data[i].college);
			printf("%s \n", leaf.data[i].name); break;
		}
	}
	if (flag == 0)
		printf("No such student exists\n");
	fseek(fp1, (table.rootpageid[1] * 512), SEEK_CUR);
	fread(&tnonleaf, sizeof(tnonleaf), 1, fp1);
	//int offset;
	for (int i = 0; i < 63; i++)
	{
		if (tnonleaf.keys[i] <= tid)
		{
			if ((i != 62 && tnonleaf.keys[i + 1]>tid) || i == 62)
			{
				flag = 1;
				offset = 64 + ((tnonleaf.offset[i + 1]) * 512);
				break;
			}
		}
		else if (tnonleaf.keys[i] > tid)
		{
			flag = 1;
			offset = 64 + ((tnonleaf.offset[i]) * 512);
			break;
		}
	}
	flag = 0;
	fseek(fp1, offset, SEEK_SET);
	fread(&staffleaf, sizeof(staffleaf), 1, fp1);
	for (int i = 0; i < 7; i++)
	{
		//printf("%d\n", staffleaf.table[i].staffid);
		if (staffleaf.table[i].staffid == tid)
		{
			flag = 1;
			printf("dept -- %s \n", staffleaf.table[i].dept);
			printf("college -- %s \n", staffleaf.table[i].college);
			printf("name -- %s \n", staffleaf.table[i].name); break;
		}
	}
	if (flag == 0)
		printf("No such teacher exists");
	_getch();
	fclose(fp);
	fclose(fp1);
	fclose(fp2);
}
*/