#include<bits/stdc++.h>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <arpa/inet.h>

#define seout cout<<"\nserver: "
#define ciout cout<<"\nclient: "

#define BUFF_SIZE 2048
using namespace std;

bool mutex_server_start=false;
int PORT;

struct client_addr
{	
	char *file_name;
	char *IP;
	int PORT;
	int start_point;
	int interval; 
};

void * download_file(void * argv)
{

		struct client_addr * client =( (struct client_addr *) argv );

		
		int sockfd = socket( AF_INET, SOCK_STREAM, 0 );
		
		struct sockaddr_in    serv_addr;

		char *file_name=client->file_name;
		int start_point=client->start_point;
		int interval = client->interval;

		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons( client->PORT );
		serv_addr.sin_addr.s_addr=inet_addr(client->IP);

		ciout;cout<<"\n Thread created for download";

		if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	    { 
	        printf("\nConnection Failed \n"); 
	        pthread_exit(NULL);
	    } 


	    ciout;
	    // send ( sockfd , &size, sizeof(size), 0);
	    sleep(1);
	    send(sockfd,&start_point ,sizeof(start_point),0);
	    sleep(1);
	    send(sockfd,file_name,strlen(file_name),0);

	    cout<<"\n start_point 00 "<<client->start_point<<" interval"<<client->interval;

	    cout<<"\n start_point 01 "<<client->start_point<<" interval"<<client->interval;
	    sleep(1);
	    send ( sockfd , &interval, sizeof(interval), 0);
	    
	    char create_file_name[256];
	    sprintf(create_file_name,"copy_%s",file_name);


		FILE *fp = fopen ( create_file_name  , "wb" );
		rewind(fp);

		char Buffer [ BUFF_SIZE] ; 
		int file_size;
		sleep(1);
		recv(sockfd, &file_size, sizeof(file_size), 0);

		cout<<"\nfile_size"<<file_size;

		int n;
		int i=client->start_point;
		

		while ( file_size > (i*BUFF_SIZE)  )
		{
			fseek(fp,i*BUFF_SIZE,SEEK_SET);
			if(( n = recv( sockfd , Buffer ,   BUFF_SIZE, 0) ) > 0)
			{	
				ciout;cout<<n<<" "<<i;

				//fseek(fp,i*BUFF_SIZE,SEEK_SET);	

				fwrite (Buffer , sizeof (char), n, fp);

				memset ( Buffer , '\0', BUFF_SIZE);
				//file_size = file_size - n;
				i+=client->interval;
			}
		} 

		fclose ( fp );
		printf("\nThread ended" );



		close( sockfd);

}


void create_null_file(struct client_addr * client)
{

	    char create_file_name[256];
	    sprintf(create_file_name,"copy_%s",client->file_name);

	    int size;
	    cout<<"\nEnter File Size\n";
	    cin>>size;

		FILE *fp = fopen ( create_file_name  , "wb" );

		char Buffer [ BUFF_SIZE] ;

		memset ( Buffer , 'a', BUFF_SIZE);
		int i=0;

		while(size>BUFF_SIZE)
		{
			if(!fwrite (Buffer , sizeof (char), BUFF_SIZE, fp))
			{
				cout<<"\nCant write into file";
			}	
			size-=BUFF_SIZE;
			cout<<i<<" "<<size<<"\n";//<<Buffer<<"\n";
			i++;
			fflush(fp);
		}
		char a='a';
		while(size--)
		{
			fwrite(&a,sizeof(char),1,fp);
		//cout<<size<<"\n";
		}



		//close(sockfd);
		fclose(fp);

		cout<<"\nEmpty File created\n";
}


void * client(void * argv)
{
	while(!mutex_server_start){
		//cout<<"\nwaiting for server part to start ";
	}

	while(1)
	{

		cout<<"\nEnter file name to share\n";
	    char *file_name = new char [256];
	    cin>>file_name;
	    cout<<"\nnumber of peer have this\n";
	    int n;
	    cin>>n;

	    std::vector<struct client_addr *> v;

	    for(int i=0;i<n;i++)
	    {
		    char *IP_to_connect=new char[20];
		    //char *IP_to_connect2=new char[20];
		    //cin>>file_name;
		    int *port=new int[1];
		    //int *port2=new int[1];
		    cout<<"\nEnter IP and PORT\n";
		    cin>>IP_to_connect;
		    cin>>*port;

		    struct client_addr *client1= new client_addr();

		    client1->IP=IP_to_connect;
		    client1->PORT=*port;
		    client1->file_name=file_name;
		    client1->start_point=i;
		    client1->interval=n;
			
			v.push_back(client1);   	
	    }
	    cout<<"i m here";
	    create_null_file(v[0]);

		pthread_t tid[n];

		for(int i=0;i<n;i++)
		{
			cout<<"\n flow will be passed to thread";
			if( pthread_create(&tid[i],NULL,&download_file,v[i]) !=0 ){
				printf("Failed to create server thread\n");
				//pthread_exit(1);		
			}
		}	// if( pthread_create(&tid_server,NULL,&fun1,&i) !=0 ){


		for(int i=0;i<n;i++)
		{
			pthread_join(tid[i], NULL);
			cout<<"\n thread Ended "<<i;
		}		

		// if( pthread_create(&tid_2,NULL,&download_file,client2) !=0 ){
		// 	printf("Failed to create client thread\n");
		// 	//pthread_exit(1);
		// }

		// pthread_join(tid_1, NULL);
		// pthread_join(tid_2, NULL);	    
		cout<<"\n All went well \n";	
	}
}

void *serveRequest(void *arg)
{

	int sockfd=*((int *)arg);

	char file_name[256];
	int interval,start_point;
	sleep(1);
	recv(sockfd, &start_point, sizeof(start_point),0);
	seout;cout<<"\nstart_point "<<start_point<<"\n";
	sleep(1);
	recv(sockfd, &file_name, sizeof(file_name),0);
	seout;cout<<"\nRequest for file "<<file_name<<"\n";


	sleep(1);
	recv(sockfd, &interval, sizeof(interval),0);
	seout;cout<<"\ninterval "<<interval<<"\n";

	sleep(1);
	FILE *fp = fopen ( file_name  , "rb" );

	fseek ( fp , 0 , SEEK_END);
	int size = ftell ( fp );
	rewind ( fp );


	send ( sockfd , &size, sizeof(size), 0);

	char Buffer [ BUFF_SIZE] ; 
	int n;
	int i = start_point;

	

	while ( size >= (i*BUFF_SIZE) )
	{
		fseek(fp,i*BUFF_SIZE,SEEK_SET);

		if( ( n = fread( Buffer , sizeof(char) , BUFF_SIZE , fp ) ) > 0)
		{
			seout;cout<<n<<" "<<i;

			//fseek( fp, i*BUFF_SIZE, SEEK_SET );

			send (sockfd , Buffer, n, 0 );

	   	 	memset ( Buffer , '\0', BUFF_SIZE);

			//size = size - i ;
	   	 	i+=interval;
		}
		else
		{
			break;
		}	//printf("\nSending File  %d \n ",size);
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

	cout<<"Enter port for process";
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