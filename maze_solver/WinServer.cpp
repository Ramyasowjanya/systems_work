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
char* path(int **arr, int rows, int columns, int sx, int sy, int dx, int dy)
{
	if (sx == dx&&sy == dy&&arr[sx][sy] != 0)
	{
		char buffer[1024];
		int index = 0,j;
		arr[sx][sy] = -1;
		for (int i = 0; i < rows; i++)
		{
			for (j = 0; j < columns-1; j++)
			{
				if (arr[i][j] == -1)
					buffer[index++] = '1';
				else
					buffer[index++] = '0';
				buffer[index++] = ',';
			}
			if (arr[i][j] == -1)
				buffer[index++] = '1';
			else
				buffer[index++] = '0';
			buffer[index++] = '\n';
		}
		buffer[index] ='\0';
		return buffer;
	}
	else if (sx < rows&&sy < columns)
	{
		if (sx + 1 < rows&&arr[sx + 1][sy] == 1)
		{
			arr[sx][sy] = -1;
			path(arr, rows, columns, sx + 1, sy, dx, dy);
		}
		else if (sy + 1 < columns&&arr[sx][sy + 1] == 1)
		{
			arr[sx][sy] = -1;
			path(arr, rows, columns, sx, sy + 1, dx, dy);
		}
		else if (sx - 1 >= 0 && arr[sx - 1][sy] == 1)
		{
			arr[sx][sy] = -1;
			path(arr, rows, columns, sx - 1, sy, dx, dy);
		}
		else if (sy - 1 >= 0 && arr[sx][sy - 1] == 1)
		{
			arr[sx][sy] = -1;
			path(arr, rows, columns, sx, sy - 1, dx, dy);
		}
	}
	else
	{
		char buffer[1024];
		int index = 0;
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columns-1; j++)
			{
				buffer[index++] = '0';
				buffer[index++] = ',';
			}
			buffer[index++] = '0';
			buffer[index++] = '\n';
		}
		buffer[index] = '\0';
		return buffer;
	}
}
int parse(char *str,int len,int start,int *dest,char match)
{
	*dest = 0;
	while (str[start] != match)
	{
		*dest = ((*dest)*10)+str[start++]-'0';
	}
	return start;
}
void process_input(char *recvbuf, int recv_buf_cnt, int* csock) 
{
	char *replybuf;
	int cols, rows,srcx,srcy,destx,desty,j,temp,**arr,len,index;
	//printf("%s",recvbuf);
	len = strlen(recvbuf);
	replybuf = (char*)malloc(len*sizeof(char));
	//printf("%d %c",len,recvbuf[1]);
	index=parse(recvbuf,len,0,&rows,'\n');
	index = parse(recvbuf, len,index+1, &cols,'\n');
	index++;
	arr = (int **)malloc(rows * sizeof(int *));
	for (int i = 0; i<rows; i++)
		arr[i] = (int *)malloc(cols * sizeof(int));
	for (int i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			arr[i][j] = recvbuf[index] - '0';
			index = index + 2;
		}
	}
	index = parse(recvbuf, len, index, &srcx, ' ');
	index = parse(recvbuf, len, index + 1, &srcy, '\n');
	index = parse(recvbuf, len, index + 1, &destx, ' ');
	desty = recvbuf[index+1]-'0';
	/*printf("enteed array\n");
	for (int i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			printf("%d ",arr[i][j]);
		}
		printf("\n");
	}*/
	replybuf=path(arr, rows, cols, srcx, srcy, destx, desty);
	//printf("%s",recvbuf);
	replyto_client(replybuf, csock);
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
	int recv_byte_cnt;

	memset(recvbuf, 0, recvbuf_len);
	if((recv_byte_cnt = recv(*csock, recvbuf, recvbuf_len, 0))==SOCKET_ERROR){
		fprintf(stderr, "Error receiving data %d\n", WSAGetLastError());
		free (csock);
		return 0;
	}

	//printf("Received bytes %d\nReceived string \"%s\"\n", recv_byte_cnt, recvbuf);
	process_input(recvbuf, recv_byte_cnt, csock);

    return 0;
}