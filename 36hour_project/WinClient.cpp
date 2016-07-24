#include <winsock2.h>
#include "stdafx.h" 
#include <windows.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <conio.h>

int getsocket()
{
	int hsock;
	int * p_int ;
	hsock = socket(AF_INET, SOCK_STREAM, 0);
	if(hsock == -1){
		printf("Error initializing socket %d\n",WSAGetLastError());
		return -1;
	}
	
	p_int = (int*)malloc(sizeof(int));
	*p_int = 1;
	if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
		(setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) ){
		printf("Error setting options %d\n", WSAGetLastError());
		free(p_int);
		return -1;
	}
	free(p_int);

	return hsock;
}

int convert_str_to_int(char *name)
{
	int number = 0, len = strlen(name), index = 0;
	while (index<len)
	{
		number = (number * 10) + name[index] - '0';
		index++;
	}
	return number;
}

int get_index(char *str, char ch)
{
	int len = strlen(str), prev = 0;
	for (int i = 0; i < len; i++)
	{
		if (str[i] == ch)
			return i;
	}
	return -1;
}

void socket_client()
{

	//The port and address you want to connect to
	int host_port= 1101;
	char* host_name="127.0.0.1";

	//Initialize socket support WINDOWS ONLY!
	unsigned short wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );
 	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 || ( LOBYTE( wsaData.wVersion ) != 2 ||
		    HIBYTE( wsaData.wVersion ) != 2 )) {
	    fprintf(stderr, "Could not find sock dll %d\n",WSAGetLastError());
		goto FINISH;
	}

	//Initialize sockets and set any options

	//Connect to the server
	struct sockaddr_in my_addr;

	my_addr.sin_family = AF_INET ;
	my_addr.sin_port = htons(host_port);
	
	memset(&(my_addr.sin_zero), 0, 8);
	my_addr.sin_addr.s_addr = inet_addr(host_name);

	//if( connect( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == SOCKET_ERROR ){
	//	fprintf(stderr, "Error connecting socket %d\n", WSAGetLastError());
	//	goto FINISH;
	//}

	//Now lets do the client related stuff
	char *buffer, name[48], sentname[70] = {""},temp,username[18];
	int buffer_len =2048;
	int bytecount;
	int c,sch=1,choice=0,len=0;
	int ct = 0, svm = 0;
//	while(true) {
		int hsock = getsocket();
		//add error checking on hsock...
		if( connect(hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == SOCKET_ERROR ){
			fprintf(stderr, "Error connecting socket %d\n", WSAGetLastError());
			goto FINISH;
		}
		scanf("%s", username);
		while (sch < 4)
		{
			printf("1.Blob store\n");
			printf("2.Message store\n");
			printf("3.Calender store\n");
			printf("4.Exit\n");
			scanf("%d", &sch);
			switch (sch)
			{
			case 1:while (choice < 5)
						{
						printf("1.Add file\n");
						printf("2.View file list\n");
						printf("3.Download file\n");
						printf("4.Delete file\n");
						printf("5.Exit\n");
						scanf("%d", &choice);
						if (choice< 5)
						{
							memset(sentname, '\0', sizeof(sentname));
							//memset(name, '\0', sizeof(name));
							//memset(username, '\0', sizeof(username));
							if (choice != 2)
							{
								printf("Enter file name\n");
								scanf("%s", name);
								fflush(stdin);
							}
							else
							{
								name[0] = '\0';
							}
							sentname[0] = sch + '0';
							sentname[1] = choice + '0';
							strcat(sentname, username);
							len = strlen(sentname);
							sentname[len] = '-';
							strcat(sentname, name);
							if ((bytecount = send(hsock, sentname, strlen(sentname), 0)) == SOCKET_ERROR)
							{
								fprintf(stderr, "Error sending data %d\n", WSAGetLastError());
								goto FINISH;
							}
							if (choice == 2)
								len = 1024;
							else
								len = 40;
							buffer = (char*)malloc(len*sizeof(char));
							memset(buffer, '\0', sizeof(buffer));
							if ((bytecount = recv(hsock, buffer, len, 0)) == SOCKET_ERROR) {
							fprintf(stderr, "Error receiving data %d\n", WSAGetLastError());
							goto FINISH;
							}
							printf("%s",buffer);
						}
					}
				break;
			case 2:
				break;
			case 3:choice = 0;
				while (choice < 2)
			{
				printf("\n1.View Categories");
				printf("\n2.Exit");
				printf("\nEnter the choice");
				scanf("%d", &choice);
				switch (choice)
				{
				case 1:sentname[0] = sch + '0';
					sentname[1] = '0';
					if ((bytecount = send(hsock, sentname, strlen(sentname), 0)) == SOCKET_ERROR)
					{
						fprintf(stderr, "Error sending data %d\n", WSAGetLastError());
						goto FINISH;
					}
					len = 110;
					buffer = (char*)malloc(len*sizeof(char));
					memset(buffer, '\0', sizeof(buffer));
					if ((bytecount = recv(hsock, buffer, len, 0)) == SOCKET_ERROR) {
						fprintf(stderr, "Error receiving data %d\n", WSAGetLastError());
						goto FINISH;
					}
					int i = 0,i1,temp;
					buffer[0] = ' ';
					while (i < len)
					{						i=get_index(buffer,'-');
						buffer[i] = ' ';
						printf("%d",buffer[i-1]+'0');
						i1 = get_index(buffer, '\n');
						if (i1 == -1)
							break;
						for (temp = i; temp < i1 + 1; temp++)
							printf("%c",buffer[temp]);
						buffer[i1] = ' ';
						i = i+3;
					}
					for (temp = i; buffer[temp]!='\0'; temp++)
						printf("%c", buffer[temp]);
					//printf("%s", buffer);
					printf("Enter category ");
					scanf("%d", &ct); svm = 0;
					int temp1 = 0, person = 0, appoint = 0;
					char name[12], uname[24], role[16], ph[11], date[11], user[20];
					while (svm < 3)
					{
						printf("1.View servicemen\n");
						printf("2.Register new servicemen\n");
						printf("3.Back\n");
						printf("Enter choice\n");
						scanf("%d", &svm);
						sentname[0] = sch + '0';
						sentname[1] = '1';
						sentname[2] = ct + '0';
						sentname[3] = svm + '0';
						sentname[4] = '\0';
						
						int  i1 = -1, prev, count = 1;
						switch (svm)
						{
						case 1:
							if ((bytecount = send(hsock, sentname, strlen(sentname), 0)) == SOCKET_ERROR)
							{
								fprintf(stderr, "Error sending data %d\n", WSAGetLastError());
								goto FINISH;
							}
							len = 400;//5 members data
							buffer = (char*)malloc(len*sizeof(char));
							memset(buffer, '\0', sizeof(buffer));
							if ((bytecount = recv(hsock, buffer, len, 0)) == SOCKET_ERROR) {
								fprintf(stderr, "Error receiving data %d\n", WSAGetLastError());
								goto FINISH;
							}
							while (i < len)
							{
								prev = i1+1;
								i1 = get_index(buffer, ':');
								if (i1 == -1)
									break;
								printf("%d.  ",count);
								for (temp = prev; temp < i1; temp++)
									printf("%c", buffer[temp]);
								printf("\n");
								buffer[i1] = ' ';
								count++;
							}
							printf("Appointments yes or no\n ");
							scanf("%d", &temp1);
							if (temp1 == 1)
							{
								printf("Enter the person\n");
								scanf("%d", &person);
								printf("1.Book an appointment\n");
								printf("2.View all appointments\n");
								printf("Enter choice");
								scanf("%d", &appoint);
								sentname[0] = '1';
								sentname[1] = appoint + '0';
								sentname[2] = person + '0';
								sentname[3] = '\0';
								switch (appoint)
								{
								case 1:
									printf("Enter the date in dd-mm-yyyy");
									scanf("%s", date);
									printf("Enter the user name");
									scanf("%s", user);
									strcat(sentname,date);
									strcat(sentname, user);
									fflush(stdin);
									if ((bytecount = send(hsock, sentname, strlen(sentname), 0)) == SOCKET_ERROR)
									{
										fprintf(stderr, "Error sending data %d\n", WSAGetLastError());
										goto FINISH;
									}
									len = 20;//5 members data
									buffer = (char*)malloc(len*sizeof(char));
									memset(buffer, '\0', sizeof(buffer));
									if ((bytecount = recv(hsock, buffer, len, 0)) == SOCKET_ERROR) {
										fprintf(stderr, "Error receiving data %d\n", WSAGetLastError());
										goto FINISH;
									}
									printf("%s\n",buffer);
									break;
								case 2:
										if ((bytecount = send(hsock, sentname, strlen(sentname), 0)) == SOCKET_ERROR)
										{
											fprintf(stderr, "Error sending data %d\n", WSAGetLastError());
											goto FINISH;
										}

									   len = 200;
									   buffer = (char*)malloc(len*sizeof(char));
									   memset(buffer, '\0', sizeof(buffer));
									   if ((bytecount = recv(hsock, buffer, len, 0)) == SOCKET_ERROR) {
										   fprintf(stderr, "Error receiving data %d\n", WSAGetLastError());
										   goto FINISH;
									   }
									   printf("%s\n", buffer);
									break;
								}
							}
							break;
						case 2:
							printf("Enter username");
							scanf("%s", uname);
							strcat(sentname,uname);
							strcat(sentname, "-");
							printf("Enter role");
							scanf("%s", role);
							strcat(sentname, role);
							strcat(sentname, "-");
							printf("Enter phoneno");
							scanf("%s", ph);
							strcat(sentname, ph);
							strcat(sentname, "-");
							if ((bytecount = send(hsock, sentname, strlen(sentname), 0)) == SOCKET_ERROR)
							{
								fprintf(stderr, "Error sending data %d\n", WSAGetLastError());
								goto FINISH;
							}
							len = 24;
							buffer = (char*)malloc(len*sizeof(char));
							memset(buffer, '\0', sizeof(buffer));
							if ((bytecount = recv(hsock, buffer, len, 0)) == SOCKET_ERROR) {
								fprintf(stderr, "Error receiving data %d\n", WSAGetLastError());
								goto FINISH;
							}
							printf("%s\n",buffer);
							break;
						case 3:
							break;
						}
					}
				}
			}
				break;
			case 4:break;
			}
		}
		_getch();

FINISH:
;
}
