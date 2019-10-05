#include<bits/stdc++.h>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <arpa/inet.h>

#define BUFF_SIZE 2048
using namespace std;

bool mutex_server_start=false;
int PORT;

struct client_addr
{
	char *IP;
	int *PORT; 
};

std::map<char *, vector<client_addr* >* > map_shared_file;

void * client(void * argv)
{
	while(!mutex_server_start){
		//cout<<"\nwaiting for server part to start ";
	}

	while(1)
	{

		cout<<"\nEnter file name to share\n";
	    char *file_name = new char [256];
	    char *IP_to_connect1=new char[20];
	    char *IP_to_connect2=new char[20];
	    cin>>file_name;
	    int *port1=new int[1];
	    int *port2=new int[1];
	    cout<<"\nEnter IP1 and PORT1\n";
	    cin>>IP_to_connect1;
	    cin>>*port1;

	    cout<<"\nEnter IP2 and PORT2\n";
	    cin>>IP_to_connect2;
	    cin>>*port2;

	    struct client_addr *client1= new client_addr();
	    struct client_addr *client2= new client_addr();
	    client1->IP=IP_to_connect1;
	    client1->PORT=port1;

	    client2->IP=IP_to_connect2;
	    client2->PORT=port2;
	    
	    vector<struct client_addr*> * v=new vector<struct client_addr*>();

	    v->push_back(client1);
	    v->push_back(client2);

	    map_shared_file.insert({file_name,v});

		// int sockfd = socket( AF_INET, SOCK_STREAM, 0 );

		// struct sockaddr_in    serv_addr;
		// serv_addr.sin_family = AF_INET;
		// serv_addr.sin_port = htons( port );
		// serv_addr.sin_addr.s_addr=inet_addr(IP_to_connect);


		// cout<<"\nport no - "<<serv_addr.sin_port<<"\n";


		// if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	 //    { 
	 //        printf("\nConnection Failed \n"); 
	 //        pthread_exit(NULL);
	 //    } 

	 //    send(sockfd,file_name,sizeof(file_name),0);

	 //    char create_file_name[256];
	 //    sprintf(create_file_name,"copy_%s",file_name);


		// FILE *fp = fopen ( create_file_name  , "wb" );
		// char Buffer [ BUFF_SIZE] ; 
		// int file_size;

		// recv(sockfd, &file_size, sizeof(file_size), 0);

		// float total_size=file_size;
		// float recieved =0;
		// int n;
		// while ( file_size > 0 && ( n = recv( sockfd , Buffer ,   BUFF_SIZE, 0) ) > 0  ){
			
		// 	recieved+=n;
		// 	fwrite (Buffer , sizeof (char), n, fp);
		// 	memset ( Buffer , '\0', BUFF_SIZE);
		// 	file_size = file_size - n;
		// 	//printf("\nRecieving File  %.2f \n ",(recieved/total_size)*100);
		// } 

		// fclose ( fp );
		// printf("\nThread ended" );


		// close( sockfd);
	}
}

void *serveRequest(void *arg)
{

	int sockfd=*((int *)arg);

	char file_name[256];

	recv(sockfd, &file_name, sizeof(file_name),0);

	//cout<<file_name<<"\n";

	FILE *fp = fopen ( file_name  , "rb" );

	fseek ( fp , 0 , SEEK_END);
	int size = ftell ( fp );
	rewind ( fp );


	send ( sockfd , &size, sizeof(size), 0);

	char Buffer [ BUFF_SIZE] ; 
	int n;

	while ( size > 0 && ( n = fread( Buffer , sizeof(char) , BUFF_SIZE , fp ) ) > 0  )
	{
			
			send (sockfd , Buffer, n, 0 );
	   	 	memset ( Buffer , '\0', BUFF_SIZE);
			size = size - n ;
			//printf("\nSending File  %d \n ",size);
	}

	cout<<"\n Sending file complete";
	fclose ( fp );	
	

	close( sockfd);
}


void * server(void * argv)
{
	cout<<"ram";

	//init();

	int server_fd = socket (AF_INET, SOCK_STREAM, 0);
	
	pthread_t tid;

	cout<<"\nEnter port for process\n";
	cin>>PORT;

	struct sockaddr_in   server_addr,client_addr;

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons( PORT );
	server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");


	cout<<"\nport no - "<<(int)server_addr.sin_port<<"\n";

	int addrlen = sizeof(sockaddr);

	if(bind (server_fd  , (struct sockaddr *)&server_addr , sizeof ( server_addr ) ) !=0)
	{
		cout<<"\nError in Binding Socket\n";
		exit(1);
	}
	mutex_server_start=true;
	listen (server_fd, 3);
	cout<<"\nListening";
	
	while(1)
	{

		int sockfd = accept ( server_fd , (struct sockaddr *)&client_addr , (socklen_t*)&addrlen);
		cout<<"\nport no - "<<client_addr.sin_port<<"\n";
		cout<<"\nConnection Established";
		
		if( pthread_create(&tid,NULL,&serveRequest,&sockfd) !=0 )
			printf("Failed to create thread\n");

	}


	close( server_fd);	
}

int main()
{
	pthread_t tid_client,tid_server;


	if( pthread_create(&tid_server,NULL,&server,NULL) !=0 ){
		printf("Failed to create server thread\n");return -1;		
	}

	if( pthread_create(&tid_client,NULL,&client,NULL) !=0 ){
		printf("Failed to create client thread\n");return -1;
	}

	pthread_join(tid_server, NULL);

	return 0;				
}