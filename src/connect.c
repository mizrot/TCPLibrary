#include"connect.h"

int establish_connection_client(socket_t clientSocket, struct sockaddr *server, socklen_t len){


  	const unsigned int MAX_TIMEOUT = 8;
	unsigned int timeout = 0;
   	while(timeout <= MAX_TIMEOUT && connect(clientSocket , server , len) == SOCKET_ERROR)
  	{
    		print_error("Connect failed. waiting");
		os_sleep(2*_TO_SEC);
		timeout += 2*_TO_SEC;
  	}      
	if (timeout == MAX_TIMEOUT){
		print_error("Maximum timeout reached");
		return -1;
	}

	return 1;
	
}

socket_t establish_connection_server(socket_t serverSocket, struct sockaddr *client, socklen_t len){

	while(listen(serverSocket, 5) == SOCKET_ERROR)
	{
	        print_error("Connect was pending too long or connection failed. waiting");
		os_sleep(2*_TO_SEC);
	}
  	socket_t sock = accept(serverSocket,client, &len);
	  if (sock == INVALID_SOCKET) {
	    print_error("Socket busy or not responding. ");
	    return INVALID_SOCKET;
	  }
  return sock;
}
