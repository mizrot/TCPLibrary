#include "../../include/connect.h"
#include <stdio.h>

int establish_connection_client(socket_t clientSocket, struct sockaddr *server){

	unsigned int timeout = 0;
   	while(timeout <= 8 && connect(clientSocket , (struct sockaddr *)server , sizeof(*server)) < 0)
  	{
    		print_error("Connect failed. waiting");
		sleep(2);
		timeout += 2;
  	}      
	
}

int establish_connection_server(socket_t serverSocket, struct sockaddr *client){
	while(listen(serverSocket, 5) == INVALID_SOCKET_VALUE)
	{
	        print_error("Connect was pending too long or connection failed. waiting");
		sleep(2);
	}
}
