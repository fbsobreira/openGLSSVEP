#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H


#ifdef __linux__ 
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
#else
	#include <winsock2.h>
	//#define _WIN32_WINNT 0x501
	#include <ws2tcpip.h>
	#include <windows.h>      // Needed for all Winsock stuff
#endif
#include <stdexcept>


class udp_client
{
public:
		udp_client(const std::string& addr, int port);
		~udp_client();

		int get_socket() const;
		int get_port() const;
		std::string get_addr() const;
		int recv(char *msg, size_t max_size);
		int timed_recv(char *msg, size_t max_size, int max_wait_time_millisecs);
		int send(char *msg, int length);
		

	private:
		int f_socket;
		int f_port;
		std::string f_addr;
		struct addrinfo* f_addrinfo;
		struct sockaddr_in server;

};

#endif // UDP_CLIENT_H
