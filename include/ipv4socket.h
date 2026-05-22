#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#ifdef _WIN32
#define close(sock) closesocket(sock);
#else
#define close(sock) close(sock);
#endif

#ifdef _WIN32
typedef SOCKET socket_t;
#else
typedef int socket_t;
#endif


socket_t create_empy_socket();
struct sockaddr_in create_ipv4(const char *str, int port);
