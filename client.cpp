
#include <string.h>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vector>
#define MAXFILE 500

using namespace std;

int main (int argc, char* argv[])
{
	int listenFd, portNo;
	bool loop = false;
	struct sockaddr_in svrAdd;
	struct hostent *server;
    
   	if(argc < 4)
	{
		fprintf(stderr, "Usage client <hostname> <port number> <timestamp> <filename>\n");
		return 0;
	}
	portNo = atoi(argv[2]);
    
	if((portNo > 65535) || (portNo < 2000))
	{
		cerr<<"Please enter port number between 2000 - 65535"<<endl;
		return 0;
	}       
    
	//create client skt
	listenFd = socket(AF_INET, SOCK_STREAM, 0);
    
	if(listenFd < 0)
	{
		cerr << "Cannot open socket" << endl;
		return 0;
	}
    
	server = gethostbyname(argv[1]);
    
	if(server == NULL)
	{
		cerr << "Host does not exist" << endl;
		return 0;
	}
    
	bzero((char *) &svrAdd, sizeof(svrAdd));
	svrAdd.sin_family = AF_INET;
    
	bcopy((char *) server -> h_addr, (char *) &svrAdd.sin_addr.s_addr, server -> h_length);
    
	svrAdd.sin_port = htons(portNo);
    
	int checker = connect(listenFd,(struct sockaddr *) &svrAdd, sizeof(svrAdd));
    
	if (checker < 0)
	{
		cerr << "Cannot connect!" << endl;
		return 0;
    	}
    
	
	
	char buffer[MAXFILE];
	int x=0;
	char ch;
	char line[200];
	char *filename;
	filename=argv[4];

	//open the filename received from the command line
	FILE *myfile = fopen(filename, "r"); 
	if(myfile==NULL)
	{
		printf("Error opening file\n");
	}                                                              

	int portno;
	double sec;
	//extract values from command line arguements
	sec = atof(argv[3]);
	

	
	double time;
	double initial;
	//send stuff to server
	
	x++;
	memset(buffer, 0, MAXFILE);
	int size=0;
	char string[MAXFILE];
	int transactions=0;
    
	//get each transaction and send it one by one
	while (fgets(buffer, MAXFILE, myfile) != NULL)
	{
    		transactions++;
    		initial=initial+1;
    		char *p;
    		p=strtok(buffer,"\n");
    		if(p)
    		{
    			strcpy(buffer,p);
    		}

    		size+=strlen(buffer);
    		//printf("%s %lu %d buffer \n", buffer,strlen(buffer),size);
	
    		char record[256];
    		strcpy(record,buffer);
	
    		//checking timestamp before sending
    		p=strtok(record," ");
    		if(p)
    		{
    			time=atoi(p);
    		}
    		//delay based on the timestamp provided in the file and the time already spent
    		
    		while(initial!=time)
    		{
    			if(initial>time)
    			{
    				sleep((initial-time)*sec);
    				initial=initial+1;
    				//printf("wait%f\n",(initial-time)*sec);
    			}
	
    		else
    			{
    				sleep((time-initial)*sec);
    				initial=initial+1;
    			}
    		
    		}
	
    		int n;
    		if(initial==time)
    		{
    			//initial=time;
    			cout << "Sending..>>"<< buffer << "\n\n"<<endl;
    			
			n = write(listenFd, buffer, sizeof(buffer));
			if(n < 0)
			{
				cerr<< "Error with writing to socket\n"<<endl;
				exit(1);	
			}
			bzero(buffer, MAXFILE);
			//read the acknowledgement received from server
			n = read(listenFd, buffer, MAXFILE);
			cout <<"Message Received : "<<buffer<<"\n"<<endl;

		}
	  	 	
	}	
	
	close(listenFd);
	exit(0); 


	return 0;
	
	
}
