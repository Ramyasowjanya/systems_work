#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
void main()
{
	FILE *fpc, *fps;
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
