#include "../../include/connect.h"

int establish_connection_client(socket_t clientSocket, struct sockaddr *server){
	unsigned int timeout = 0;
   	while(timeout <= 8000 && connect(clientSocket , (struct sockaddr *)server , sizeof(*server)) == SOCKET_ERROR)
  	{
    		print_error("Connect failed. waiting");
		Sleep(2000);
		timeout += 2000;
  	}      
	
}

int establish_connection_server(socket_t serverSocket, struct sockaddr *client){
	while(listen(serverSocket, 5) == INVALID_SOCKET_VALUE)
	{
	        print_error("Connect was pending too long or connection failed. waiting");
		Sleep(2000);
	}
}
