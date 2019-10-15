#include<bits/stdc++.h>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <arpa/inet.h>
using namespace std;

char traker_Ip[20];
int tracker_port;

int PORT;
bool mutex_server_start=false;
char * uid = new char [256];
#define BUFF_SIZE 2048

struct cmp_str
{
   bool operator()(char const *a, char const *b) const
   {
      return std::strcmp(a, b) < 0;
   }
};


struct struct_file_details
{
	int size;
	int no_of_chunks;
	char * bit_vector;
};
std::map<char * , struct struct_file_details *,cmp_str> map_file_details;



#include "client.h"
#include "server.h"

int main()
{
	pthread_t tid_client,tid_server;

	cout<<"\nEnter tracker IP and PORT";
	cin>>traker_Ip>>tracker_port;

	strcpy(uid,"");

	if( pthread_create(&tid_server,NULL,&server,NULL) !=0 ){
		printf("Failed to create server thread\n");return -1;		
	}

	if( pthread_create(&tid_client,NULL,&client,NULL) !=0 ){
		printf("Failed to create client thread\n");return -1;
	}

	pthread_join(tid_server, NULL);

	return 0;				
}


// 111
// 111
// 111
// 111
// 111
// 111
// 111
// 0

// 111111111111111111111
// 1000000000000000000000