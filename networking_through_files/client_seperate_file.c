#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

void main()
{
	FILE *fpc, *fps;
	fpc = fopen("client.txt", "w");
	int ch= 0; char data, input[50];
	fps = fopen("D:/vs/serverfiles/serverfiles/server.txt", "r");
	while(1)
		{
			fpc = fopen("client.txt", "w");
			//fseek(fpc, 0, SEEK_SET);
			printf("\nEnter any message");
			gets(input);
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
}
