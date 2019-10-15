#include<bits/stdc++.h>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <arpa/inet.h>
#define CHUNK_SIZE 512*1024
#define seout cout<<"\nserver: "



void *serveRequest(void *arg)
{

	int sockfd=*((int *)arg);

	char file_name[256];
	int interval,start_point;
	bool ack=true;

	
	recv(sockfd, &start_point, sizeof(start_point),0);
	send(sockfd,&ack,sizeof(ack),0);
	seout;cout<<"\nstart_point "<<start_point<<"\n";
	
	
	memset ( file_name , '\0', 256);
	recv(sockfd, &file_name, sizeof(file_name),0);
	send(sockfd,&ack,sizeof(ack),0);
	seout;cout<<"\nRequest for file "<<file_name<<"\n";


	recv(sockfd, &interval, sizeof(interval),0);
	send(sockfd,&ack,sizeof(ack),0);

	seout;cout<<"\ninterval "<<interval<<"\n";

	FILE * fp = fopen ( file_name  , "rb" );

	if ( fp == NULL )
	{
	    printf( "Could not open file test.c" ) ;
	    //return 1;
	}	

	fseek ( fp , 0 , SEEK_END);
	int size = ftell ( fp );
	rewind ( fp );

	auto itr=map_file_details.find(file_name);
	struct struct_file_details * f = itr->second;

	cout<<"\n"<<f->bit_vector;

	send ( sockfd , &size, sizeof(size), 0);
	recv(sockfd,&ack,sizeof(ack),0);

	int number_of_chunks=((size-1)/(CHUNK_SIZE))+1;

	send(sockfd,f->bit_vector,strlen(f->bit_vector),0);
	recv(sockfd,&ack,sizeof(ack),0);


	
	char bit_vector[number_of_chunks]="";
	recv(sockfd,&bit_vector,sizeof(bit_vector),0);
	cout<<"\nno of chunks "<<number_of_chunks;
	cout<<"\n\tbitvector-";

	for(int i=0;i<number_of_chunks;i++)
		cout<<bit_vector[i];
	cout<<"\n";



	char Buffer[BUFF_SIZE] ; 
	int n=0;
	//int i = start_point;

	
	for(int i=0;i<number_of_chunks;i++)
	{
		fseek(fp,i*CHUNK_SIZE,SEEK_SET);
		char big_Buffer[CHUNK_SIZE];

		if(bit_vector[i]=='1')
		{
			seout;cout<<"\n sending i chunk-"<<i;
			memset(big_Buffer,'\0',CHUNK_SIZE);
			int no_of_bytes_sended=0;
			int ack=0;
			while ( CHUNK_SIZE > no_of_bytes_sended )
			{
				//fseek(fp,i*BUFF_SIZE,SEEK_SET);

				if( ( n = fread( Buffer , sizeof(char) , BUFF_SIZE , fp ) ) > 0)
				{
					//n = fread( Buffer , sizeof(char) , BUFF_SIZE , fp );
					seout;cout<<n;
					send (sockfd , Buffer, n, 0 );
			   	 	seout;cout<<n<<" "<<i<<" "<<ack<<" "<<n<<" "<<no_of_bytes_sended;

			   	 	memset ( Buffer , '\0', BUFF_SIZE);

			   	 	recv( sockfd , &ack ,sizeof(ack), 0);
					//size = size - i ;
			   	 	//i+=interval;
			   	 	no_of_bytes_sended+=BUFF_SIZE;
				}
				else
				{	seout;cout<<"breaking while loop";
					break;
				}	//printf("\nSending File  %d \n ",size);
			}
		}
	}
	cout<<"\n Sending file complete";
	// fclose ( fp );	
	

	close( sockfd);
}


void * server(void * argv)
{
	cout<<"ram";

	//init();

	int server_fd = socket (AF_INET, SOCK_STREAM, 0);
	
	pthread_t tid;

	// cout<<"Enter port for process";
	// cin>>PORT;

	struct sockaddr_in   server_addr,client_addr;

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons( PORT );
	server_addr.sin_addr.s_addr=inet_addr(SERVER_IP);


	cout<<"\nServer port no - "<<ntohs(server_addr.sin_port)<<"\n";

	int addrlen = sizeof(sockaddr);

	if(bind (server_fd  , (struct sockaddr *)&server_addr , sizeof ( server_addr ) ) !=0)
	{
		seout;cout<<"\nError in Binding Socket\n";
		exit(1);
	}
	mutex_server_start=true;
	listen (server_fd, 3);
	cout<<"\nListening";
	
	while(1)
	{

		int sockfd = accept ( server_fd , (struct sockaddr *)&client_addr , (socklen_t*)&addrlen);
		cout<<"\nport no - "<<ntohs(client_addr.sin_port)<<"\n";
		cout<<"\nIp add  - "<<inet_ntoa(client_addr.sin_addr);
		cout<<"\nConnection Established";
		
		if( pthread_create(&tid,NULL,&serveRequest,&sockfd) !=0 )
			printf("Failed to create thread\n");

	}


	close( server_fd);	
}
