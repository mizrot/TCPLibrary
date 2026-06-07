#ifndef TCP_PLATFORM_H
#define TCP_PLATFORM_H

#include <stdbool.h>
#include <stdio.h>

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
#define close_socket(socket) closesocket(socket)
#else
#define close_socket(socket) close(socket)
#endif

#ifndef _WIN32
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#endif

#ifdef _WIN32
#define SHUTDOWN SD_BOTH
#else
#define SHUTDOWN SHUT_RDWR
#endif

// Error handle
#ifdef _WIN32
#define print_error(msg)                                                       \
  printf("%s. Windows Error: %d\n", msg, WSAGetLastError());
#else
#define print_error(msg) perror(msg)
#endif

#ifdef _WIN32
#define _TO_SEC 1000
#else
#define _TO_SEC 1
#endif

#ifdef _WIN32
#define os_sleep(s) Sleep(s)
#else
#define os_sleep(s) sleep(s)
#endif

#ifdef _WIN32
#define PLATFORM_WINDOWS
#else
#define PLATFORM_LINUX
#endif

#endif
