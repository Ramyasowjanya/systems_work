#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
void main()
{
	FILE *fpc, *fps;
	fps = fopen("server.txt", "w");
	int ch=0; char data, *input;
	
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
		gets(input);
		ch = 1;
		fprintf(fps, "%d%s\n", ch, input);
		free(input);
		fclose(fps);
	}
	fclose(fps);
	_getch();

}
