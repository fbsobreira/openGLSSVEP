#include "udp_client.h"

#include <string.h>
#include <unistd.h>
#ifdef __linux__ 
#include <sys/fcntl.h> // for non-blocking
#endif

#include <stdio.h>
	class udp_client_runtime_error : public std::runtime_error
	{
		public:
			udp_client_runtime_error(const char *w) : std::runtime_error(w){}
	};
	
#ifndef __linux__ 
// inet_aton for windows
int inet_aton(const char *address, struct in_addr *sock){
     
    int s;
    s = inet_addr(address);
    if ( s == 1 && strcmp (address, "255.255.255.225") )
    return 0;
    sock->s_addr = s;
     
    return 1;
}
#endif

/**
	*Here we need to initialize the client object.
	*We'll use a fixed port and try first doing client-client protocol on the same computer.
	*If that works we can try moving code afterwards.
	*The port specified is host side. Let's use something like 59,200.
	*The addr parameter represents a textual address, either IPv6 or v6.
	* It can either directly be the host name or just the numbers- let's use numbers.
	*On failure to resolve the address, the client will error and the constructor will throw an exception.
	* Don't fork() or exec() in this, it'll close the socket!
	* Constructor time!
	* \param addr : The address to convert to a numeric IP.
	* \param port : The port number to use for the connection.
	*/
udp_client::udp_client(const std::string& addr, int port) : f_port(port), f_addr(addr)
{
	char decimal_port[16];
	snprintf(decimal_port, sizeof(decimal_port), "%d", f_port);
	decimal_port[sizeof(decimal_port) / sizeof(decimal_port[0]) - 1] = '\0';
	
	
	/* Clear out server struct */
	memset((void *)&server, '\0', sizeof(struct sockaddr_in));
	/* Set family and port */
	server.sin_family = AF_UNSPEC;
	server.sin_port = htons(port);
	/* Set server address */
	inet_aton("127.0.0.1", &server.sin_addr);	
	
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	int r(getaddrinfo(addr.c_str(), decimal_port, &hints, &f_addrinfo)); //automatically get the info for the address given, error # stored in r
	if (r != 0 || f_addrinfo == NULL) { //if we couldn't get the info or we got an error return (-1), didn't work.
		throw udp_client_runtime_error(("Invalid address or port given: " + addr + ":" + decimal_port).c_str());
	}
	#ifdef __linux__ 
    f_socket = socket(f_addrinfo->ai_family, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP);
	#else
	f_socket = socket(f_addrinfo->ai_family, SOCK_DGRAM, IPPROTO_UDP);
	#endif
	
	if (f_socket == -1) {
		freeaddrinfo(f_addrinfo);
		throw udp_client_runtime_error(("Could not create a socket for " + addr + ":" + decimal_port).c_str());
	}
	
	
}

/**
*\brief This is the destructor for the UPD client. It'll free the address info struct and close the socket before
* it returns.
*/
udp_client::~udp_client()
{
	freeaddrinfo(f_addrinfo);
	close(f_socket);
}

/**
* \brief This will return a copy of the socket identifier, which you can use to change flags if you want.
*/
int udp_client::get_socket() const
{
	return f_socket;
}

/**
* \brief This will return the port being used by the client.
*/
int udp_client::get_port() const
{
	return f_port;
}

/** \brief Retrieve a copy of the address being used as a string.
*
* This function returns a copy of the address as it was specified in the
* constructor. This does not return a canonalized version of the address.
*
* The address cannot be modified. If you need to send data on a different
* address, create a new UDP client.
*
* \return A string with a copy of the constructor input address.
*/
std::string udp_client::get_addr() const
{
	return f_addr;
}

/** \brief Wait on a message.
*
* This function waits until a message is received on this UDP client.
* There are no means to return from this function except by receiving
* a message. Remember that UDP does not have a connect state, so if
* the server quits or disconnects, it does not change the status of this UDP client
* and thus it will continue to wait forever.
*
* Note that you may change the type of socket by making it non-blocking
* (use the get_socket() to retrieve the socket identifier) in which
* case this function will not block if no message is available. Instead
* it returns immediately.
*
* As written with infinite blocking, this is a good function to use for debugging since if you
* experience an infinite wait, you know that something is wrong in that the client never received a message.
* \param[in] msg  The buffer where the message is saved.
* \param[in] max_size  The maximum size the message (i.e. size of the \p msg buffer.)
*
* \return The number of bytes read or -1 if an error occurs.
*/
int udp_client::recv(char *msg, size_t max_size)
{
	return ::recv(f_socket, msg, max_size, 0);
}

int udp_client::send(char *msg, int length)
{
	sendto(f_socket, msg, length, 0, (struct sockaddr *)&server,  sizeof(server));
}

/** \brief Wait for data to come in.
*
* This function waits for a given amount of time for data to come in.If
* no data comes in after max_wait_ms, the function returns with - 1 and
* errno set to EAGAIN.
*
* The socket is expected to be a blocking socket(the default, ) although
* it is possible to setup the socket as non - blocking if necessary for
* some other reason.
*
* This function blocks for a maximum amount of time as defined by
* max_wait_ms.It may return sooner with an error or a message.
*
* \param[in] msg  The buffer where the message will be saved.
* \param[in] max_size  The size of the \p msg buffer in bytes.
* \param[in] max_wait_ms  The maximum number of milliseconds to wait for a message.
*
* \return -1 if an error occurs or the function timed out, the number of bytes received otherwise.
*/
int udp_client::timed_recv(char *msg, size_t max_size, int max_wait_ms)
{
	fd_set s;
	FD_ZERO(&s);
	FD_SET(f_socket, &s);
	struct timeval timeout;
	timeout.tv_sec = max_wait_ms / 1000;
	timeout.tv_usec = (max_wait_ms % 1000) * 1000;
	int retval = select(f_socket + 1, &s, &s, &s, &timeout);
	if (retval == -1) {
		// select() set errno accordingly
		return -1;
	}
	if (retval > 0) {
		// our socket has data
		return ::recv(f_socket, msg, max_size, 0);
	}

	// our socket has no data
	errno = EAGAIN;
	return -1;
}
