#include "../../include/connect.h"

int establish_connection_client(socket_t clientSocket, struct sockaddr *server){


  	const unsigned int MAX_TIMEOUT = 8;
	unsigned int timeout = 0;
   	while(timeout <= MAX_TIMEOUT && connect(clientSocket , (struct sockaddr *)server , sizeof(*server)) == SOCKET_ERROR)
  	{
    		print_error("Connect failed. waiting");
		Sleep(2000);
		timeout += 2000;
  	}      
	if (timeout == MAX_TIMEOUT){
		print_error("Maximum timeout reached");
		return -1;
	}

	return 1;
	
}

int establish_connection_server(socket_t serverSocket, struct sockaddr *client){

	while(listen(serverSocket, 5) == INVALID_SOCKET_VALUE)
	{
	        print_error("Connect was pending too long or connection failed. waiting");
		Sleep(2000);
	}
	
	return 1;
}
