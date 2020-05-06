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
#include <pwd.h>
#include <stdbool.h>
#include <syslog.h>
#include <time.h>
#include "rot13.h"
#include "auth.h"

#define BUF_SIZE 1024

/**
 * @brief  This is the server agent for linux sockets programming example.
 *         1) server is started
 *         2)    Each (TCP) accept() call is followed by a fork() to handle concurrent connections.
 *         3) Client sends 1 or more username strings to server
 *         4) server replies with 'valid' or 'invlaid' username 
 *         5) Each connection response for username validity is logged to the server host syslog.
 * @note   Assumes all connections are done to/from this host, localhost.
 *         IPv6 has not been tested. "Should" but may not display IP address strings correctly.
 *         Supports concurrent client connections for TCP or UDP. Test successully with 1000 clients.
 * @param  argc: required command line args
 * @param  **argv: command line args to launch server
 * 
 *                  format: ./server <port number> <socket type (TCP | UDP)>
 * 
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
 * @retval int - EXIT_SUCCESS | EXIT_FAILURE
 */
int main(int argc, char **argv)
{
	   struct addrinfo *result = NULL;
           struct addrinfo *rp = NULL;
           struct addrinfo hints;
           int sfd, s, socket_type, listen_ret;
           ssize_t nread;
           char buf[BUF_SIZE];
	   int accept_fd;
	   char ipstr[INET6_ADDRSTRLEN];
           struct sockaddr_storage peer_addr;
           socklen_t peer_addr_len = sizeof(struct sockaddr_storage);
           char host[NI_MAXHOST], service[NI_MAXSERV];

	   /* Verify command line args */
	   if (argc != 3) { 
		   fprintf(stderr, "Usage: %s port socktype(TCP or UDP)\n", argv[0]);
                   exit(EXIT_FAILURE);
           }

	   if (0 == strcmp(argv[2], "TCP")) { 
		   socket_type = SOCK_STREAM;
           }
	   else if (0 == strcmp(argv[2], "UDP")) { 
		   socket_type = SOCK_DGRAM;
	   }
	   else {
		   assertf(0, "socktype %s not supported by this service", argv[2]);
	   }

	    /* Setup hints structure for getaddrinfo() to get address(es) 
	     * matching host/port 
	     */
           memset(&hints, 0, sizeof(struct addrinfo));
           hints.ai_family = AF_UNSPEC;     /* Allow IPv4 or IPv6 */
           hints.ai_socktype = socket_type; /* TCP or UDP */
           hints.ai_flags = AI_PASSIVE;     /* Get server IP for free (localhost) */
           hints.ai_protocol = 0;           /* Any protocol */
           hints.ai_canonname = NULL;
           hints.ai_addr = NULL;
           hints.ai_next = NULL;

	   /* Set 'node' arg to NULL because we intend to accept connections
	    * only from clients on this same host. 'localhost' will become
		* 0.0.0.0 or 127.0.0.1. 
	    */
	   s = getaddrinfo(NULL, argv[1], &hints, &result);
           assertf(s == 0, "[Server] failed gettaddrinfo(): %s", gai_strerror(s));

	   /** getaddrinfo() returns a list of address structures. Try each address
       *  until there is a successful bind.
       */
       for (rp = result; rp != NULL; rp = rp->ai_next) {
           void *addr = NULL;
	       char *ipver = NULL;
           sfd = socket(rp->ai_family, rp->ai_socktype,
                        rp->ai_protocol);
           if (sfd == -1)
                continue;

           struct sockaddr_in *ipv4 = (struct sockaddr_in *)rp->ai_addr;
	       addr = &(ipv4->sin_addr);
	       ipver = "IPv4";

	       /* Convert the IP to a string and print it */
	       inet_ntop(rp->ai_family, addr, ipstr, sizeof ipstr);
           printf("  %s: %s\n", ipver, ipstr);

            if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0) {
		        printf ("Successful bind.\n");
                break;                  /* Success */
		   }
           close(sfd);
        }
        if (rp == NULL) {               /* No address succeeded */
           fprintf(stderr, "Could not bind to an address\n");
           exit(EXIT_FAILURE);
        }

       /* free up list of address(es) */
	   freeaddrinfo(result);

       /* Support specific to TCP traffic */
	   if (socket_type == SOCK_STREAM) { 
		   listen_ret = listen(sfd, 5);
	       assertf( listen_ret == 0, "[Server] failed listen()");

		   pid_t pid;
	       while (1) { 
			   accept_fd = accept(sfd, (struct sockaddr *)&peer_addr, 
					              &peer_addr_len); 
			   assertf(accept_fd != -1, "[Server] failed accept()");
			   pid = fork();
	           assertf( pid != -1, "[Server] failed fork()");
			   if (pid == 0) { // child
				   close(sfd);
				   printf ("new child (pid %d) using descriptor %d\n", getpid(), accept_fd);
                   s = getnameinfo((struct sockaddr *) &peer_addr,
                                   peer_addr_len, host, NI_MAXHOST,
                                   service, NI_MAXSERV, NI_NUMERICSERV);
		            auth_service_tcp(accept_fd, host, service);
			        exit (EXIT_SUCCESS);
			    }
			   else { // parent 
				   close(accept_fd);
			   }
	        }
	   }
       /* Support specific to UDP traffic */
	   else { 

	        unsigned char *valid="Valid username";
	        unsigned char *invalid="Invalid username";
		    while (1)  {
			   nread = recvfrom(sfd, buf, BUF_SIZE, 0,
                                (struct sockaddr *) &peer_addr, &peer_addr_len);
                if (nread == -1)
				   continue;            

                /* Create host and service/port strings for useful output */
                s = getnameinfo((struct sockaddr *) &peer_addr,
                                 peer_addr_len, host, NI_MAXHOST,
                                 service, NI_MAXSERV, NI_NUMERICSERV);
                if (s == 0) {
				   printf("[Server] Received %zd bytes from %s:%s %s\n", nread, host, service, buf);
			   }
               else {
				   fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));
			   }

			   /** Deterimine username validity from client and append result to
				* syslog.
				*/
		       if (auth(buf)) { 

				   append_syslog(valid);
		           if (sendto(sfd, valid, BUF_SIZE, 0, 
				      (struct sockaddr *) &peer_addr, peer_addr_len) != BUF_SIZE) { 
				         fprintf(stderr, "Error sending response\n"); 
			        }
			   }
			   else { 
				   append_syslog(invalid);
		                   if (sendto(sfd, invalid, BUF_SIZE, 0, 
				      (struct sockaddr *) &peer_addr, 
			              peer_addr_len) != BUF_SIZE) 
			            { 
				      fprintf(stderr, "Error sending response\n"); 
			            }
			   }

		   }
	   }
	   exit(EXIT_SUCCESS);
} 

/**
 * @brief Fn to determine if client username is valid on this host.
 *        This fn write a response back to the client and appends the
 *        syslog with the validity result.
 * 
 * @param fd - The accept_fd file descriptor
 * @param host - localhost
 * @param service - port number
 */
void auth_service_tcp (int fd, char *host, char *service) {
	unsigned char buf[1024];
	unsigned char *valid="Valid username";
	unsigned char *invalid="Invalid username";
	int count;

 	 while ((count = read(fd, buf, BUF_SIZE)) > 0) { 
			printf("[Server] Received %d msg bytes from %s:%s:%d: %s\n", count, host, service, fd, buf);
		        if (auth(buf)) { 
				write(fd, valid, BUF_SIZE);
		                append_syslog(valid);
		        }
		        else { 
				write(fd, invalid, BUF_SIZE);
		                append_syslog(invalid);
		        }
		}
}

/**
 * @brief Fn to simply validate username sent to server from client.
 * 
 * @param buf - client msg buffer contents (username)
 * @return true  - valid username
 * @return false - invalid username
 */
bool auth(char *buf) {
	struct passwd pwd;
	bool retval = false;
	if (NULL != getpwnam(buf))
		retval = true;
	return retval;
}

/**
 * @brief Fn to append the host syslog with client username validity check result.
 * 
 * @param username_response - invalid or valid
 */
void append_syslog(const char *username_response) {
   time_t ltime;
   ltime = time(NULL);
   openlog(NULL, LOG_PID, LOG_USER);
   syslog(LOG_INFO, "%s: %d: %s", asctime(localtime(&ltime)), getpid(), username_response);
   closelog();
}

