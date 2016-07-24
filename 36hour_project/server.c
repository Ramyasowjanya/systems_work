#include "stdafx.h"
#include <winsock2.h>
#include <windows.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
struct node
{
	char msg[128];
	int msg_id;
	node *next;
}*flist,*alist,*printid;

struct filedata//74 bytes
{
	int bit;
	char name[48];
	char user[18];
	int len;
};

struct servicemen//58 bytes
{
	char name[24];
	char role[16];
	char phoneno[11];
	int nextmen;
	int appointment;
};
struct appointments//35 bytes
{
	char date[11];
	char user[20];
	int nextappointment;
};
struct category//16 bytes
{
	char name[12];
	int offset;
};

struct bufserv{
	
		int userId;
		int forumId;
		int msgId;
		int commentId;
		int choice;
		char *forumname;
		char msg[128];
}buf1;

bool flag=true;
int mid = 0;
int count1 =0;
char *Data[100];
int count=1;
int values[100];
DWORD WINAPI SocketHandler(void*);
void replyto_client(char *buf, int *csock);


int search_infile(FILE *fp, char *name)
{
	struct filedata available;
	int position;
	fseek(fp, 0, SEEK_SET);
	while (!feof(fp))
	{
		position = ftell(fp);
		fread(&available, sizeof(available), 1, fp);
		if (available.bit != 0 && !strcmp(available.name, name))
		{
			return position;
		}
		fseek(fp, available.len, SEEK_CUR);
	}
	return -1;
}

void add_file(FILE *fp, FILE *fp1, int length, int setoffset, char *name,char *user,int *csock)
{
	struct filedata available;
	char *data,*replybuf;
	fseek(fp, setoffset, SEEK_SET);
	strcpy(available.name, name);
	strcpy(available.user, user);
	available.len = length;
	available.bit = 1;
	fwrite(&available, sizeof(available), 1, fp);
	data = (char*)malloc(length*sizeof(char));
	memset(data, '\0', sizeof(data));
	fseek(fp1, 0, SEEK_SET);
	fread(data, length, 1, fp1);
	fwrite(data, length, 1, fp);
	length = 0;
	fclose(fp1);
	free(data);
	replybuf = (char*)malloc((11)*sizeof(char));
	memset(replybuf, '\0', sizeof(replybuf));
	strcat(replybuf, "File added\n");
	length = strlen(replybuf);
	replybuf[length] = '\0';
	replyto_client(replybuf, csock);
	//printf("File added\n");
}

void fill_in_deleted(FILE *fp, FILE *fp1, char *name, int length,char *user,int *csock)
{
	struct filedata available;
	int position;
	char *replybuf;
	fseek(fp, 0, SEEK_SET);
	while (!feof(fp))
	{
		position = ftell(fp);
		fread(&available, sizeof(available), 1, fp);
		if (available.bit == 2 && available.len >= length)
		{
			add_file(fp, fp1, length, position, name,user,csock);
			//printf("File added\n");
			return;
		}
		fseek(fp, available.len, SEEK_CUR);
	}
	replybuf = (char*)malloc((20)*sizeof(char));
	memset(replybuf, '\0', sizeof(replybuf));
	strcat(replybuf, "File cannot be added\n");
	length = strlen(replybuf);
	replybuf[length] = '\0';
	replyto_client(replybuf, csock);
	//printf("File cannot be added\n");
}

int check_available(FILE *fp,char *name)
{
	struct filedata available;
	int position;
	fseek(fp, 0, SEEK_SET);
	while (!feof(fp))
	{
		position = ftell(fp);
		fread(&available, sizeof(available), 1, fp);
		if (available.bit != 0 && !strcmp(available.name, name))
		{
			return -2;
		}
		if (available.bit == 0)
		{
			return position;
		}
		fseek(fp, available.len, SEEK_CUR);
	}
	return -1;
}
void strcopy(char *src, char *dest, int end, int start)
{
	int i = 0;
	for (i = start; i < end; i++)
		dest[i - start] = src[i];
	dest[i - start] = '\0';
}
int search_user_files(FILE *fp, char *name,int *csock)
{
	struct filedata available;
	char data[1024];
	memset(data,'\0',sizeof(char));
	int position=1;
	fseek(fp, 0, SEEK_SET);
	while (!feof(fp))
	{
		fread(&available, sizeof(available), 1, fp);
		if (available.bit ==1 && !strcmp(available.user, name))
		{
			strcat(data, available.name);
			position =position+ strlen(available.name);
			data[position] = '\n';
			data[position+1] = '\0';
			fseek(fp, available.len, SEEK_CUR);
		}
	}
	if (position != 0)
	{
		//data[position + 1] = '\0';
		replyto_client(data, csock);
		return 0;
	}
	return -1;
}
void socket_server() {

	//The port you want the server to listen on
	int host_port= 1101;

	unsigned short wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD( 2, 2 );
 	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 || ( LOBYTE( wsaData.wVersion ) != 2 ||
		    HIBYTE( wsaData.wVersion ) != 2 )) {
	    fprintf(stderr, "No sock dll %d\n",WSAGetLastError());
		goto FINISH;
	}

	//Initialize sockets and set options
	int hsock;
	int * p_int ;
	hsock = socket(AF_INET, SOCK_STREAM, 0);
	if(hsock == -1){
		printf("Error initializing socket %d\n",WSAGetLastError());
		goto FINISH;
	}
	
	p_int = (int*)malloc(sizeof(int));
	*p_int = 1;
	if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
		(setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) ){
		printf("Error setting options %d\n", WSAGetLastError());
		free(p_int);
		goto FINISH;
	}
	free(p_int);

	//Bind and listen
	struct sockaddr_in my_addr;
	my_addr.sin_family = AF_INET ;
	my_addr.sin_port = htons(host_port);
	
	memset(&(my_addr.sin_zero), 0, 8);
	my_addr.sin_addr.s_addr = INADDR_ANY ;
	
	/* if you get error in bind 
	make sure nothing else is listening on that port */
	if( bind( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1 ){
		fprintf(stderr,"Error binding to socket %d\n",WSAGetLastError());
		goto FINISH;
	}
	if(listen( hsock, 10) == -1 ){
		fprintf(stderr, "Error listening %d\n",WSAGetLastError());
		goto FINISH;
	}
	
	//Now lets do the actual server stuff

	int* csock;
	sockaddr_in sadr;
	int	addr_size = sizeof(SOCKADDR);
	
	while(true){
		printf("waiting for a connection\n");
		csock = (int*)malloc(sizeof(int));
		
		if((*csock = accept( hsock, (SOCKADDR*)&sadr, &addr_size))!= INVALID_SOCKET ){
			//printf("Received connection from %s",inet_ntoa(sadr.sin_addr));
			CreateThread(0,0,&SocketHandler, (void*)csock , 0,0);
			//printf("%d--csock\n",*csock);
		}
		else{
			fprintf(stderr, "Error accepting %d\n",WSAGetLastError());
		}
	}

FINISH:
;
}
int get_index(char *str,char ch)
{
	int len = strlen(str),prev=0;
	for (int i = 0; i < len; i++)
	{
		if (str[i] == ch)
			prev = i;
	}
	return prev;
}
int get_start_index(char *str, char ch)
{
	int len = strlen(str), prev = 0;
	for (int i = 0; i < len; i++)
	{
		if (str[i] == ch)
			return i;
	}
	return -1;
}
void process_input(char *recvbuf,int recv_buf_cnt, int* csock) 
{
	char *replybuf, name[48], username[18],tname[20];
	FILE *fp, *fp1,*fpc;
	char *data, nametemp[70] = { "C:/Users/Anjan/" }, downloadname[70] = { "C:/Users/Anjan/" };
	int returnval,index;
	struct filedata available;
	int setoffset, length, temp = 2;
	switch (recvbuf[0]-'0')
	{
	case 1:
		index = get_index(recvbuf,'-');
			strcopy(recvbuf, username, index, 2);
			strcopy(recvbuf, name, strlen(recvbuf), index+1);
			fp = fopen("blobdata.bin","rb+");
			switch (recvbuf[1] - '0')
			{
			case 1:
				index = get_index(name, '/');
				strcopy(name, tname, strlen(name), index + 1);
				setoffset = check_available(fp,tname);
				fp1 = fopen(name, "rb");
				fseek(fp1, 0, SEEK_END);
				length = ftell(fp1);
				if (setoffset != -1)
				{
				add_file(fp, fp1, length, setoffset, tname,username,csock);
				}
				else if(setoffset!=-2)
				{
					fill_in_deleted(fp, fp1, tname, length,username,csock);
				}
				else
				{
					replybuf = (char*)malloc((20)*sizeof(char));
					memset(replybuf, '\0', sizeof(replybuf));
					strcat(replybuf, "File already exists\n");
					length = strlen(replybuf);
					replybuf[length] = '\0';
					replyto_client(replybuf, csock);
				}
				break;
			case 2:
				returnval=search_user_files(fp,username,csock);
				if(returnval==-1)
				{
					replybuf = (char*)malloc((20)*sizeof(char));
					memset(replybuf, '\0', sizeof(replybuf));
					strcat(replybuf, "No such file exists\n");
					length = strlen(replybuf);
					replybuf[length] = '\0';
					replyto_client(replybuf, csock);
				}
				break;
			case 3:
				returnval = search_infile(fp, name);
				if (returnval != -1)
				{
					fseek(fp, returnval, SEEK_SET);
					fread(&available, sizeof(available), 1, fp);
					data = (char*)malloc((available.len)*sizeof(char));
					memset(data, '\0', sizeof(data));
					fread(data, available.len, 1, fp);
					strcat(downloadname, name);
					fp1 = fopen(downloadname, "wb");
					fwrite(data, available.len, 1, fp1);
					fclose(fp1);
					replybuf = (char*)malloc((19+strlen(downloadname))*sizeof(char));
					memset(replybuf, '\0', sizeof(replybuf));
					strcat(replybuf, "File downloaded at ");
					strcat(replybuf,downloadname);
					length = strlen(replybuf);
					replybuf[length] = '\0';
					replyto_client(replybuf, csock);
					//printf("File downloaded at %s\n", downloadname);
					strcpy(downloadname, nametemp);
				}
				else
				{
					replybuf = (char*)malloc((20)*sizeof(char));
					memset(replybuf, '\0', sizeof(replybuf));
					strcat(replybuf, "No such file exists\n");
					length = strlen(replybuf);
					replybuf[length] = '\0';
					replyto_client(replybuf, csock);
				}
				break;
			case 4:returnval = search_infile(fp, name);
				if (returnval != -1)
				{
					fseek(fp, returnval, SEEK_SET);
					temp = 2;
					fwrite(&temp, sizeof(temp), 1, fp);
					replybuf = (char*)malloc((13)*sizeof(char));
					memset(replybuf, '\0', sizeof(replybuf));
					strcat(replybuf, "File deleted\n");
					length = strlen(replybuf);
					replybuf[length] = '\0';
					replyto_client(replybuf, csock);
					//printf("File deleted\n");
				}
				else
				{
					replybuf = (char*)malloc((20)*sizeof(char));
					memset(replybuf,'\0',sizeof(replybuf));
					strcat(replybuf,"No such file exists\n");
					length = strlen(replybuf);
					replybuf[length] = '\0';
					replyto_client(replybuf, csock);//printf("No such file exists");
				}
				break;
			}
			fclose(fp);
			break;
	case 2:break;
	case 3:
		int i = 0, ct = 0, svm = 0, temp = 0,index=0, position = 0, appoint = 0, person = 0, temp1 = 0;
		char name[12], uname[24], role[16], ph[11], date[11], user[20],ch[4];
		struct category ctg;
		struct appointments appointment, appointment1;
		struct servicemen men, men1;
		fpc = fopen("calenderdata.bin", "rb+");
		i = 0;
		switch (recvbuf[1] - '0')
		{
		case 0:	fseek(fpc, 4, SEEK_SET);
			replybuf = (char*)malloc((160)*sizeof(char));
			memset(replybuf, '\0', sizeof(replybuf));
			while (i < 10)
			{
				memset(&ctg, 0, sizeof(ctg));
				fread(&ctg, sizeof(ctg), 1, fpc);
				//replybuf[index+2]=(i+1)-'0' ;
				ch[0] = '\n';
				ch[1] = (i + 1)-'0';
				ch[2] = '-';
				ch[3] = '\0';
				strcat(replybuf,ch);
				strcat(replybuf,ctg.name);
				//printf("%d.%s\n", i + 1, ctg.name);
				i++;
			}
			replyto_client(replybuf, csock);
			break;
		case 1:ct= recvbuf[2] - '0';
			svm = recvbuf[3] - '0';
			switch(svm)
			{
			case 1:
				replybuf = (char*)malloc((400)*sizeof(char));
				memset(replybuf, '\0', sizeof(replybuf));
				fseek(fpc, 4 + (ct - 1) * 16, SEEK_SET);
				fread(&ctg, sizeof(ctg), 1, fpc);
				if (ctg.offset == -1)
				{
					strcat(replybuf, "No service men exists");
					replyto_client(replybuf,csock);
				}
				else
				{
					fseek(fpc, ctg.offset, SEEK_SET);
					fread(&men, sizeof(men), 1, fpc);
					strcat(replybuf, men.name);
					strcat(replybuf, "\t");
					strcat(replybuf, men.role);
					strcat(replybuf, "\t");
					strcat(replybuf, men.phoneno);
					i = strlen(replybuf);
					replybuf[i] = ':';
					replybuf[i + 1] = '\0';
					while (men.nextmen != -1)
					{
						if (men.nextmen != -1)
							fseek(fpc, men.nextmen, SEEK_SET);
						fread(&men, sizeof(men), 1, fpc);
						strcat(replybuf, men.name);
						strcat(replybuf,"\t");
						strcat(replybuf, men.role);
						strcat(replybuf, "\t");
						strcat(replybuf, men.phoneno);
						i = strlen(replybuf);
						replybuf[i] = ':';
						replybuf[i + 1] = '\0';
					}
				}
				replyto_client(replybuf, csock);
				memset(recvbuf, 0, 1024);
				if ((recv(*csock, recvbuf, 1024, 0)) == SOCKET_ERROR) {
					fprintf(stderr, "Error receiving data %d\n", WSAGetLastError());
					free(csock);
				}
				fseek(fpc, 0, SEEK_SET);
				//position = ftell(fp);
				fread(&position, sizeof(position), 1, fpc);
				fseek(fpc, 4 + (ct - 1) * 16, SEEK_SET);
				fread(&ctg, sizeof(ctg), 1, fpc);
				switch(recvbuf[0] - '0')
				{
				case 1:
					switch (recvbuf[1] - '0')
					{
					case 1:i = 1;
						person = recvbuf[2] - '0';
						strcopy(recvbuf,date,13,3);
						strcopy(recvbuf,user,strlen(recvbuf),13);
						fseek(fpc, ctg.offset, SEEK_SET);
						fread(&men, sizeof(men), 1, fpc);
						temp = ftell(fpc) - sizeof(men);
						while (men.nextmen != -1 && i < person)
						{
							if (men.nextmen != -1)
								fseek(fpc, men.nextmen, SEEK_SET);
							temp = ftell(fpc);
							fread(&men, sizeof(men), 1, fpc);
						}
						fseek(fpc, 0, SEEK_SET);
						fread(&position, sizeof(position), 1, fpc);
						if (men.appointment == -1)
						{
							men.appointment = position;
							fseek(fpc, temp, SEEK_SET);
							fwrite(&men, sizeof(men), 1, fpc);
							strcpy(appointment.date, date);
							strcpy(appointment.user, user);
							appointment.nextappointment = -1;
							fseek(fpc, position, SEEK_SET);
							fwrite(&appointment, sizeof(appointment), 1, fpc);
						}
						else
						{
							fseek(fpc, men.appointment, SEEK_SET);
							temp = ftell(fpc);
							fread(&appointment, sizeof(appointment), 1, fpc);
							fseek(fpc, 0, SEEK_SET);
							fread(&position, sizeof(position), 1, fpc);
							while (appointment.nextappointment != -1)
							{
								if (appointment.nextappointment != -1)
									fseek(fpc, appointment.nextappointment, SEEK_SET);
								temp = ftell(fpc);
								fread(&appointment, sizeof(appointment), 1, fpc);
							}
							appointment.nextappointment = position;
							fseek(fpc, temp, SEEK_SET);
							fwrite(&appointment, sizeof(appointment), 1, fpc);
							strcpy(appointment1.date, date);
							strcpy(appointment1.user, user);
							appointment1.nextappointment = -1;
							fseek(fpc, position, SEEK_SET);
							fwrite(&appointment1, sizeof(appointment1), 1, fpc);
						}
						position = ftell(fpc);
						fseek(fpc, 0, SEEK_SET);
						fwrite(&position, sizeof(position), 1, fpc);
						replyto_client("Booked appointment", csock);
						break;
					case 2:
						person = recvbuf[2] - '0';
						replybuf=(char*)malloc(200*sizeof(char));
						memset(replybuf,'\0',sizeof(replybuf));
						fseek(fpc, ctg.offset, SEEK_SET);
						fread(&men, sizeof(men), 1, fpc); i = 1;
						temp = ftell(fpc) - sizeof(men);
						while (men.nextmen != -1 && i < person)
						{
							if (men.nextmen != -1)
								fseek(fpc, men.nextmen, SEEK_SET);
							temp = ftell(fpc);
							fread(&men, sizeof(men), 1, fpc);
							i++;
						}
						if (men.appointment != -1)
						{
							fseek(fpc, men.appointment, SEEK_SET);
							fread(&appointment, sizeof(appointment), 1, fpc);
							strcat(replybuf, appointment.date);
							strcat(replybuf, " - ");
							strcat(replybuf, appointment.user);
							strcat(replybuf, "\n");
							while (appointment.nextappointment != -1)
							{
								if (appointment.nextappointment != -1)
									fseek(fpc, appointment.nextappointment, SEEK_SET);
								temp = ftell(fpc);
								fread(&appointment, sizeof(appointment), 1, fpc);
								strcat(replybuf, appointment.date);
								strcat(replybuf, " - ");
								strcat(replybuf, appointment.user);
								strcat(replybuf, "\n");
							}
							replybuf[strlen(replybuf)] = '\0';
							replyto_client(replybuf, csock);
						}
						else
							replyto_client("No appointments", csock);
						break;
					}
					break;
				}
				break;
			case 2:
				memset(uname,'\0',sizeof(uname));
				memset(role, '\0', sizeof(role));
				memset(ph, '\0', sizeof(ph));
				temp1 = 0; i = 3;
				while (temp1 < 3)
				{
					temp = i + 1;
					i = get_start_index(recvbuf, '-');
					switch (temp1)
					{
					case 0:strcopy(recvbuf,uname,i,temp);
						break;
					case 1:strcopy(recvbuf, role, i, temp);
						break;
					case 2:strcopy(recvbuf, ph, i, temp);
						break;
					}
					recvbuf[i] = ' ';
					temp1++;
				}
				replybuf = (char*)malloc((24)*sizeof(char));
				memset(replybuf, '\0', sizeof(replybuf));
				fseek(fpc, 0, SEEK_SET);
				fread(&position, sizeof(position), 1, fpc);
				fseek(fpc, 4 + (ct - 1) * 16, SEEK_SET);
				fread(&ctg, sizeof(ctg), 1, fpc);
				memset(&men, 0, sizeof(men));
				strcpy(men.name, uname);
				men.appointment = -1;
				men.nextmen = -1;
				strcpy(men.role, role);
				strcpy(men.phoneno, ph);
					if (ctg.offset == -1)
					{
						fseek(fpc, 4 + (ct - 1) * 16, SEEK_SET);
						ctg.offset = position;
						fwrite(&ctg, sizeof(ctg), 1, fpc);
						fseek(fpc, position, SEEK_SET);
						fwrite(&men, sizeof(men), 1, fpc);
					}
				   else
				   {
					   fseek(fpc, ctg.offset, SEEK_SET);
					   temp = ctg.offset;
					   fread(&men1, sizeof(men1), 1, fpc);
					   while (men1.nextmen != -1)
					   {
						  // printf("%s %s %s \n", men1.name, men1.role, men1.phoneno);
						   if (men1.nextmen != -1)
							   fseek(fpc, men1.nextmen, SEEK_SET);
						   temp = ftell(fpc);
						   fread(&men1, sizeof(men1), 1, fpc);
					   }
					   fseek(fpc, temp, SEEK_SET);
					   men1.nextmen = position;
					   fwrite(&men1, sizeof(men1), 1, fpc);
					   fseek(fpc, position, SEEK_SET);
					   fwrite(&men, sizeof(men), 1, fpc);
				   }
				   position = ftell(fpc);
				   fseek(fpc, 0, SEEK_SET);
				   fwrite(&position, sizeof(position), 1, fpc);
				   replyto_client("Registered successfully",csock);
				break;
			}
			break;
		}
		fclose(fpc);
		break;
	}
	//replyto_client(replybuf, csock);
}

void replyto_client(char *buf, int *csock) {
	int bytecount;
	
	if((bytecount = send(*csock, buf, strlen(buf), 0))==SOCKET_ERROR){
		fprintf(stderr, "Error sending data %d\n", WSAGetLastError());
		free (csock);
	}
	printf("replied to client: %s\n",buf);
}

DWORD WINAPI SocketHandler(void* lp){
    int *csock = (int*)lp;

	char recvbuf[1024];
	int recvbuf_len = 1024;
	char recvbuf1[1024];
	int recvbuf_len1 = 1024;
	int recv_byte_cnt;

	while (1)
	{
		memset(recvbuf, 0, recvbuf_len);
		if ((recv_byte_cnt = recv(*csock, recvbuf, recvbuf_len, 0)) == SOCKET_ERROR) {
			fprintf(stderr, "Error receiving data %d\n", WSAGetLastError());
			free(csock);
			return 0;
		}
		//printf("Received bytes %d\nReceived string \"%s\"\n", recv_byte_cnt, recvbuf);
		process_input(recvbuf, recv_byte_cnt, csock);

	}
    return 0;
}

