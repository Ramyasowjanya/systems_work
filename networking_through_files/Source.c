#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
/*void main()
{
	FILE *fpc, *fps;
	fps = fopen("server.txt", "w");
	int ch=0; char data, input[50];
	fpc = fopen("D:/vs/clientfiles/clientfiles/client.txt", "r");
	while (1)
	{
		fscanf(fpc, "%d", &ch);
		while (ch != 1)
		{
			printf("%d-",ch);
			//fclose(fpc);
			//fpc = fopen("D:/vs/clientfiles/clientfiles/client.txt", "r");
			fseek(fpc, 0, SEEK_SET);
			fscanf(fpc, "%d", &ch);
		}
		fseek(fps, 0, SEEK_SET);
		ch = 0;
		fprintf(fps, "%d", ch);
		printf("\nClient replied : ");
		data = fgetc(fpc);
		while (data != '\n')
		{
			printf("%c", data);
			data = fgetc(fpc);
		}
		fseek(fps, 0, SEEK_SET);
		printf("Enter any message\n");
		scanf("%[^\n]s", input);
		ch = 1;
		//fwrite(&ch, sizeof(ch), 1, fps);
	//	fwrite(input, sizeof(input), 1, fps);
		fprintf(fps, "%d%s", ch, input); 
		
		//fwrite(&ch, sizeof(ch), 1, fps);
		//fclose(fps);
		//fps = fopen("server.txt", "a");
	}
	_fcloseall();
}*/
void main()
{
	FILE *fpc, *fps;
	fps = fopen("server.txt", "w");
	int ch=0; char data, *input;
	
	//fscanf(fpc, "%d", &ch);
	while (1)
	{
		fpc = fopen("D:/vs/clientfiles/clientfiles/client.txt", "r+");
		input = (char*)malloc(50*sizeof(char));
		fseek(fpc, 512, SEEK_SET);
		fscanf(fpc, "%d", &ch);
		//fseek(fpc, 0, SEEK_SET);
		ch = 0;
		if (ch != 1)
		{
			//ch = 0;
			fprintf(fpc, "%d", ch);
			fclose(fpc);
		}
		fpc = fopen("D:/vs/clientfiles/clientfiles/client.txt", "r+");
		while (ch != 1)
		{	
			fseek(fpc, 0, SEEK_SET);
			fscanf(fpc, "%d", &ch);
		}
		printf("\nClient replied : ");
		data = fgetc(fpc);
		while (data != '\n'&&!feof(fpc))
		{
			printf("%c", data);
			data = fgetc(fpc);
		}
		fpc = fopen("D:/vs/clientfiles/clientfiles/client.txt", "r+");
		fseek(fpc, 512, SEEK_SET);
		printf("\nEnter any message");
		//scanf("%[^\n]s", input);
		gets(input);
		ch = 1;
		fprintf(fpc, "%d%s\n", ch, input);
		free(input);
		fclose(fpc);
	}
	_getch();
}
//two files
/*
while (1)
	{
		input = (char*)malloc(50*sizeof(char));
		fseek(fpc, 0, SEEK_SET);
		fscanf(fpc, "%d", &ch);
		//fseek(fpc, 0, SEEK_SET);
		ch = 0;
		if (ch != 1)
		{
			fps = fopen("server.txt", "w");
			//ch = 0;
			fprintf(fps, "%d", ch);
			fclose(fps);
		}
		while (ch != 1)
		{	
			fseek(fpc, 0, SEEK_SET);
			fscanf(fpc, "%d", &ch);
		}
		printf("\nClient replied : ");
		data = fgetc(fpc);
		while (data != '\n'&&!feof(fpc))
		{
			printf("%c", data);
			data = fgetc(fpc);
		}
		fps = fopen("server.txt", "w");
		fseek(fps, 0, SEEK_SET);
		printf("\nEnter any message");
		//scanf("%[^\n]s", input);
		gets(input);
		ch = 1;
		fprintf(fps, "%d%s\n", ch, input);
		free(input);
		fclose(fps);
	}
	fclose(fps);
	_getch();
*/