#ifndef TCP_PLATFORM_H
#define TCP_PLATFORM_H

#include <stdio.h>
#include <stdbool.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#endif


#ifdef _WIN32
typedef SOCKET socket_t;
#else
typedef int socket_t;
#endif

#ifdef _WIN32
#define close(socket) closesocket(socket)
#else
#define close(socket) close(socket)
#endif

#ifndef _WIN32
#define INVALID_SOCKET  (-1)
#define SOCKET_ERROR    (-1)
#endif

// Error handle
#ifdef _WIN32
#define print_error(msg) printf("%s. Windows Error: %d\n", msg, WSAGetLastError());
#else
#define print_error(msg) perror(msg)
#endif

#ifdef _WIN32 
#define PLATFORM WINDOWS
#else 
#define PLATFORM LINUX
#endif

#endif
