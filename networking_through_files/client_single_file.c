#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

void main()
{
	FILE *fpc, *fps;
	fpc = fopen("client.txt", "w");
	int ch= 0; char data, input[50];
	fprintf(fpc, "%d", ch);
	fseek(fpc, 512, SEEK_SET);
	fprintf(fpc, "%d", ch); 
	fseek(fpc, 0, SEEK_END);
	ch = ftell(fpc);
	printf("%d",ch);
	_getch();
	_fcloseall();
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
