#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
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
	
};

#endif // UDP_SERVER_H
