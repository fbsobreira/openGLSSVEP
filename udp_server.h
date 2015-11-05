#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#ifdef __linux__ 
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netdb.h>
#else
	#include <windows.h>      // Needed for all Winsock stuff
	//#define _WIN32_WINNT 0x501
	#include <ws2tcpip.h>
#endif
#include <stdexcept>

class udp_server
{
public:
						udp_server(const std::string& addr, int port);
                        ~udp_server();
	
	int                 get_socket() const;
    int                 get_port() const;
    std::string         get_addr() const;

    int                 recv(char *msg, size_t max_size);
    int                 timed_recv(char *msg, size_t max_size, int max_wait_ms);
	
	int					HAS_DATA();

private:
    int                 f_socket;
    int                 f_port;
    std::string         f_addr;
    struct addrinfo *   f_addrinfo;\
	
	int					new_data;
	
	#ifndef __linux__ 
	WORD wVersionRequested = MAKEWORD(1,1);       // Stuff for WSA functions
	WSADATA wsaData;                              // Stuff for WSA functions
	#endif
	
};

#endif // UDP_SERVER_H
