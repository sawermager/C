#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctype.h>
#include <string.h>
#include "assertf.h"

#define BUF_SIZE 1024

/**
 * @brief This is the client agent for linux sockets programming example.
 *        1) Client is started and sends username message(s) to server on same host, localhost.
 *        2) Client receives response from server result of username(s) validity on localhost.
 * @param argc - required command line args
 * @param argv - command line args to launch client and connect to server
 *             
 *               format: ./client localhost <port number> <socket type (TCP|UDP)> <username string> ... 
 * Sever Example: ./server 61000 TCP
 * SERVER OUTPUT:
 * $ server 61000 TCP
 * IPv4: 0.0.0.0
 * Successful bind.
 * new child (pid 51981) using descriptor 4
 * fd: 4: Received 5 bytes: root
 * fd: 4: Received 6 bytes: rooot
 * 
 * Client Example:./server 61000 TCP root rooot
 * 
 * CLIENT OUTPUT:
 * [Client] Received 1024 bytes: Valid username for root
 * [Client] Received 1024 bytes: Invalid username for rooot
 * 
 * 
 * SYSLOG OUTPUT:
 * May  6 13:39:06 sawermager-System-Product-Name server[59443]: Wed May  6 13:39:06 2020#012: 59443: Valid username
 * May  6 13:39:06 sawermager-System-Product-Name server[59443]: Wed May  6 13:39:06 2020#012: 59443: Invalid username 
 * 
 * 
 * @return int - EXIT_SUCCESS | EXIT_FAILURE
 */
int main(int argc, char **argv) {
	struct addrinfo *result = NULL;
	struct addrinfo *rp = NULL;
	struct addrinfo hints;
	int sock_fd, cnt, connect_ret;
	int sfd, s, j, socket_type;
	size_t len;
	ssize_t nread;
	char buf[BUF_SIZE]; 

	/* Verify command line args */
	if (argc < 5) {
	    fprintf(stderr, "Usage: %s host port socktype(TCP or UDP) username ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

	if (0 == strcmp(argv[3], "TCP")) {
	    socket_type = SOCK_STREAM;
    }
    else if (0 == strcmp(argv[3], "UDP")) {
            socket_type = SOCK_DGRAM;
    }
    else {
        assertf(0, "socktype %s not supported by this service", argv[3]);
    }

	/* Setup hints structure for getaddrinfo() to get address(es) 
	 * matching host/port 
	 */
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;     /* Allow IPv4 or IPv6 */
	hints.ai_socktype = socket_type; /* TCP or UDP socket */
	hints.ai_flags = 0;              /* No wildcards specified */
	hints.ai_protocol = 0;           /* Any protocol */

    s = getaddrinfo(argv[1], argv[2], &hints, &result);
	assertf(s == 0, "[Client] failed gettaddrinfo(): %s", gai_strerror(s));

	/** getaddrinfo() returns a list of address structures. Try each address
	 *  until there is a successful connection.
	 */
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype,
                    rp->ai_protocol);
        if (sfd == -1)
            continue;
        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break;                  /* Success */
        close(sfd);
    }

    if (rp == NULL) {               /* No address succeeded */
		fprintf(stderr, "Could not connect to an address\n");
        exit(EXIT_FAILURE);
    }

    /* free up list of address(es) */
    freeaddrinfo(result); 

	 /* Send remaining command-line arguments separately 
      * to the server, then read and display the response.
	  * This allows multiple messages/transactions to be sent
	  * from same client.
	  */
         for (j = 4; j < argc; j++) {
             len = strlen(argv[j]) + 1;
             if (len > BUF_SIZE) {
                 fprintf(stderr, "Ignoring message in argument %d that exceeds buffer limit\n", j);
                 continue;
             }

             if (write(sfd, argv[j], len) != len) {
                 fprintf(stderr, "partial/failed write\n");
                 exit(EXIT_FAILURE);
             }
	     
	     /* Read response back from server */
             nread = read(sfd, buf, BUF_SIZE);
             if (nread == -1) {
                 perror("read");
                 exit(EXIT_FAILURE);
             }
             printf("[Client] Received %zd bytes: %s for %s\n", nread, buf, argv[j]);
         }
	exit(EXIT_SUCCESS);
} 



	


