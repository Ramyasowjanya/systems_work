#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
/*void main()
{
	FILE *fpc,*fps;
	fpc = fopen("client.txt","w");
	int ch; char data,input[50];
	fps = fopen("D:/vs/serverfiles/serverfiles/server.txt", "r");
	while (1)
	{
		fseek(fpc, 0, SEEK_SET);
		printf("Enter any message\n");
		scanf("%[^\n]s", input);
		ch = 1;
		fprintf(fpc, "%d%s", ch, input);
		//fwrite(&ch,sizeof(ch),1,fpc);
		//fwrite(input, sizeof(input), 1, fpc);
		//fwrite(&ch, sizeof(ch), 1, fpc);
		//fclose(fpc);
		//fpc = fopen("client.txt", "a");
		ch = 0;
		fscanf(fps, "%d", &ch);
		while (ch != 1)
		{
			//fps = fopen("D:/vs/serverfiles/serverfiles/server.txt", "r");
			fseek(fps, 0, SEEK_SET);
			fscanf(fps, "%d", &ch);
		}
		printf("\nServer replied : ");
		data = fgetc(fps);
		while (data != '\n')
		{
			printf("%c", data);
			data = fgetc(fps);
		}
		fseek(fpc, 0, SEEK_SET);
		ch = 1;
		fprintf(fpc, "%d", ch);
	}
	_fcloseall();
}*/
void main()
{
	FILE *fpc, *fps;
	fpc = fopen("client.txt", "w");
	int ch= 0; char data, input[50];
	fps = fopen("D:/vs/serverfiles/serverfiles/server.txt", "r");
	//for single file
	/*fprintf(fpc, "%d", ch);
	fseek(fpc, 512, SEEK_SET);
	fprintf(fpc, "%d", ch); 
	fseek(fpc, 0, SEEK_END);
	ch = ftell(fpc);
	printf("%d",ch);
	_getch();
	_fcloseall();*/
	while(1)
		{
			fpc = fopen("client.txt", "r+");
			printf("\nEnter any message");
			gets(input);
			ch = 1;
			fprintf(fpc, "%d%s\n", ch, input);
			fclose(fpc);
			fpc = fopen("client.txt", "r+");
			fseek(fpc, 0, SEEK_SET);
			fscanf(fpc, "%d", &ch);
			ch = 0;
			if (ch != 1)
			{
				ch = 0;
				fprintf(fpc, "%d", ch);
				fclose(fpc);
			}
			fpc = fopen("client.txt", "r+");
				while (ch != 1)
				{
				fseek(fpc, 512, SEEK_SET);
				fscanf(fpc, "%d", &ch);
				}
			printf("\nServer replied : ");
			data = fgetc(fpc);
			while (data != '\n'&&!feof(fpc))
			{
				printf("%c", data);
				data = fgetc(fpc);
			}
			fclose(fpc);
		}
		_getch();
}

//for two files
/*	while(1)
		{
			fpc = fopen("client.txt", "w");
			//fseek(fpc, 0, SEEK_SET);
			printf("\nEnter any message");
			gets(input);
			//scanf("%[^\n]s", input);
			ch = 1;
			fprintf(fpc, "%d%s\n", ch, input);
			fclose(fpc);
			fseek(fps, 0, SEEK_SET);
			fscanf(fps, "%d", &ch);
			ch = 0;
			if (ch != 1)
			{
				fpc = fopen("client.txt", "w");
				ch = 0;
				fprintf(fpc, "%d", ch);
				fclose(fpc);
			}
				while (ch != 1)
				{
				fseek(fps, 0, SEEK_SET);
				fscanf(fps, "%d", &ch);
				}
			printf("\nServer replied : ");
			data = fgetc(fps);
			while (data != '\n'&&!feof(fps))
			{
				printf("%c", data);
				data = fgetc(fps);
			}
		}
		fclose(fps);
		_getch();
	}*/