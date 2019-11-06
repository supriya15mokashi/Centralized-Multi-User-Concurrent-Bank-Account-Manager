A. Steps to run the application

	Step 1: Open two windows Terminal-1 and Terminal-2 for Server and Client respectively

	Step 2: Compile the server side code on Terminal-1 by using: 
		g++ -o server server.cpp  -std=c++11 -pthread -std=c++1z

	Step 3: Compile the client side code on Terminal-2 by using:
		g++ -o client client.cpp

	Step 4: Run the server on Terminal-1 by using:
		./server 12343

	Step 5: Run the client on Terminal-2 by using: 
		./client 127.0.0.1 12343 0.7 Transactions.txt

B. Program design

1. The program aims at executing a Centralized Multi-User Concurrent Bank Account Manager. 
2.  It has 2 main components namely, the server side and the client side 
3. The server is serving for the following purposes: 
	a. Accept the Records.txt file which includes the account number, name and the account balance for all the entries. 
	b. Maintain a log of all the customer records accurately. (Allowing withdrawals only if the account has enough balance) 
	c. Service the online requests sent by the clients for all the available entries. 
	d. Perform the operations: withdraw and deposit amount from/in an account. 
	e. Accept multiple concurrent customer requests from the clients which are multithreaded. 
	f. Provide the locking/protection mechanism while the account is being accessed simultaneously by multiple clients at same time. This is done by protecting the concerned variables by using locks in the critical sections. 
	g. Ensure the correctness of the transaction records by deducting only when balance is available.
	h. Send the status of the transaction to the clients after each operation is performed. 
4. The client code is serving for the following purposes: 
	a. Accept the Transactions.txt file which accepts the Timestamp, account number, transaction type and the amount to be deposited/withdrawn.
	b. Issue withdrawal and deposit requests to the server using the account numbers. 
	c. Receive the status of the transaction from server and show the status for each.

C. Working

1. The server first reads the Records.txt file and splits the string of line accepted. It bifurcates the accepted string line to separate the account number, name and the account balance for all the entries. It stores these values into 3 different arrays: id [], name [], balance []. 
2. Further, both the client and the server create socket. The client calls the connect function and then the server listens to the requests sent by the clients.
3. The server is a listening entity and it binds to the connection request sent by the client at port number 12343. 
4. Threading is used to allow multiple clients request the server. Each time the main function in the server code passes the connection handler function to the threads created. This way the main function is always free to listen to the client requests. 
5. In the server program, multithreading and synchronization works as follows: The main function in the servers reads from the 'Records.txt' file to store the existing data in arrays. The main function creates the socket for listening to the incoming connections. As soon as any connection is established to a defined port, the main function passes that socket connection to the 'handle_connection' function as a thread. And the control comes back to main to listen to new connections. 
6. Also the created connections keep listening to the messages from the binded IP. All the threads are joined in the process. The handle_connection function handles the operation on any thread by locking it while performing it. So that the synchronous operations are not performed at a single time and data consistency is maintained.



