#include <string.h>		// Including libraries which are required to execute various funtions in this code 
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include<cctype>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <pthread.h>
#include <thread>
#include <math.h> 
#include <cassert>
#include <vector>
#include <sstream>
#include <iterator>
#include <mutex>   


std::mutex mtx;  
#define MAXDATASIZE 256

using namespace std;

std::vector<std::thread> threads;

void handle_connection(int );

int i = 0;
int accid, amount;
char op[2];
int x = 0;
char* buffer[MAXDATASIZE];
int lines = 0;
char* records[MAXDATASIZE];
int id[MAXDATASIZE], balance[MAXDATASIZE];
char* name[MAXDATASIZE];

//fstream myfile;
fstream myfile("Records.txt");

template <size_t N>
void splitString(string (&arr)[N], string str)
{
	int n = 0;
	istringstream iss(str);
	for (auto it = istream_iterator<string>(iss); it != istream_iterator<string>() && n < N; ++it, ++n)
	arr[n] = *it;
}


int main(int argc, char* argv[])
{
	int pId, portNo, listenFd;
	socklen_t len; 				//store size of the address
	bool loop = false;
	struct sockaddr_in svrAdd, clntAdd;
	
	char ch;
	 
	 
	
	 
	string line;
	//fstream myfile ("Records.txt");

	if (myfile.is_open())
	{
		while ( getline (myfile,line) )
		{
      
			//cout << line << '\n';
			string arr[4];

			splitString(arr, line);

			/* for (int i = 0; i < 4; i++)
			cout << arr[i] << endl; */
			std::string::size_type sz;
			// records = line.Split(' ');
	 
	  
			id[i] = std::stoi (arr[0],&sz);//();	 
	   
			string str;
			str = arr[1];
		
			name[i] = &str[0];
			strcpy(name[i], arr[1].c_str());	
		
			balance[i] = std::stoi (arr[2],&sz);	 

	  
	  		i++;
    		}
		myfile.close();
	} 
	lines = i;
	

	myfile.close();
   
    
	if (argc < 2)
	{
        	cerr << "Syntam : ./server <port>" << endl;
		return 0;
	}
    
	portNo = atoi(argv[1]);
    
	if((portNo > 65535) || (portNo < 2000))
	{
		cerr << "\n Please enter a port number within 2000 - 65535" << endl;
		return 0;
	}
    
	//create socket
	listenFd = socket(AF_INET, SOCK_STREAM, 0);
    
	if(listenFd < 0)
	{
        	cerr << "\n Cannot open the socket!" << endl;
        	return 0;
	}
    
	bzero((char*) &svrAdd, sizeof(svrAdd));
    
	svrAdd.sin_family = AF_INET;
	svrAdd.sin_addr.s_addr = INADDR_ANY;
	svrAdd.sin_port = htons(portNo);
    
	//bind socket
	if(bind(listenFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd)) < 0)
	{
        	cerr << "\n Cannot bind the socket!" << endl;
        	return 0;
	}
    
	listen(listenFd, 5);
    
	len = sizeof(clntAdd);
	int connFd; 
	while (connFd =accept(listenFd, (struct sockaddr *)&clntAdd, &len))
	{
		cout << "\n Server is listening now..";

        	//this is where client connects. svr will hang in this mode until client conn
        	//connFd = accept(listenFd, (struct sockaddr *)&clntAdd, &len);

		if (connFd < 0)
        	{
			cerr << "\n Could not accept the connection request from client.." << endl;
			return 0;
        	}
        	else
        	{
			cout << "\n Connected to the client successfully.." << endl;
        	}
		
		threads.push_back(std::thread(&handle_connection, connFd));
	
		
	}
	for (auto&& t : threads)
			t.join();
   
    
}

void handle_connection (int  connFd)
{
	char contents[300];
	bzero(contents, 301);
	bool loop = false;
	int n=0;
	//n=read(newsocket, contents, MAXDATASIZE)
	while(n=read(connFd, contents, 300))
	{    
		if (n < 0) 
		{
			fprintf(stderr, "\n Error in receiving data\n");
			exit(1);
		}

		if(strlen(contents)==0)
		{
			continue;
		}

		//read(connFd, contents, 300);
		
		cout<<"\n Data received from client is as follows: "<< contents;
		cout << "\n Thread number to which client connected is: " << pthread_self();
        
		
		//separating account id, transaction to be performed and the amount from the received data
		char* p;
		p = strtok(contents, " ");
		p = strtok(NULL, " ");

		if (p) 
		{
			accid = atoi(p);
			
		}

		p = strtok(NULL, " ");

		if (p) 
		{
                	strcpy(op, p);
				
		}

		p = strtok(NULL, " ");
	
 		if (p) 
		{
			amount = atoi(p);
		}
	
	
		int size = 0;
	
		int exist = 0;
			
		for (i = 0; i < lines; i++) 
		{
	
        	        if (i == 0) 
        	        {
        	            size = 0;
        	        }
        	        else 
        	        {
        	            size += (floor(log10(abs(id[i - 1]))) + 1) + strlen(name[i - 1]) + (floor(log10(abs(balance[i - 1]))) + 1) + 3;
        	        }
					
					
        	        if (accid == id[i]) 
        	        {
				
				mtx.lock();
				cout << "\n This account ID exists in the Record! ";
	
				//Withdrawal Operation
				if (strcmp(op, "w") == 0) 
				{
					cout<<"\n Withdrawing amount..";
					if (balance[i] - amount > 0) 
					{
						balance[i] = balance[i] - amount;
						cout<<"\n Account balance is now: "<< balance[i]<<"\n\n";
	
						//writing the newly calculated balance in the file
						//std::fseek(myfile, size, SEEK_SET); //set the stream pointer i bytes from the start.
						//fprintf(myfile, "%d %s %d\n", id[i], name[i], balance[i]);
						myfile <<  id[i]<< "  "<< name[i]<<"  " << balance[i] <<" \n"<<endl;
						cout << "\n Acknowldging the client about the withdrawl.. \n";
	
  						/* Write a response to the client */
						n = write(connFd, "\n ACK: Amount withdrawn successfully! ", 50);
						if (n < 0)
							cerr<< "\n Error in writing!";
	
					}
	
					else 
					{
						//balance insufficient to perform the withdrawal
						cout<<"\n Sorry! Insufficient balance to withdraw the amount!"<< balance[i];
						cout<<"\n Acknowledgement Sent\n";

						n = write(connFd, "\n Negative ACK: Insufficient balance - Could not withdraw the amount!", 50);
						if (n < 0)
							cerr << "\n Error in writing!\n"<<endl;
					}
				}

				//Deposit Operation
				else if (strcmp(op, "d") == 0) 
				{
	
					cout<<"\n Depositing amount.. ";
					balance[i] = balance[i] + amount;
					cout<<"\n Account balance is now: " << balance[i] <<"\n\n\n";
		
					//writing the newly calculated balance in the file
					// std::fseek(myfile, size, SEEK_SET); //set the stream pointer i bytes from the start i.e. beginning of line
 					myfile <<  id[i]<< " "<< name[i]<<" " << balance[i] <<"\n"<<endl;
					cout << "\n Acknowldging the client about the deposit.. ";
		
                	        	/* Write a response to the client */
					n = write(connFd, "\n ACK: Amount deposited successfully! ", 50);
					if (n < 0)
						cerr << "\n Error in writing!"<<endl;
				}	
			
				else 
				{
			
					printf("\n Invalid operation type! Please enter a valid operation type. ");
					/* Write a response to the client */
					n = write(connFd, "\n Negative ACK: Invalid operation type ", 50);
					if (n < 0)
						cerr << "\n Error writing to socket"<<endl;
				}	
		
				exist = 1;
		
				//release the lock
				mtx.unlock();
			}	
	
		}	
	
		if (exist == 0) 
		{
			// case handling: The requested id is invalid
			cout<<"\n This account id does not exist in the records! \n"<<endl;
			n = write(connFd, "NACK\n", 4);
			if (n < 0)
				cerr << "\n Error writing to socket\n"<<endl;
		}        	
	}
        
}
