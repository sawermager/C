
/* Number-guessing server. This is an example of a 
 * single-process, concurrent server using TCP and select()
 * with file descriptor sets.
 */

#define _GNU_SOURCE
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "assertf.h"

/* Our "well-known" port number */
#define GUESSING_PORT 	61000

int process_request(int fd);

/* This is where we store the per-client state (the number to guess).
 * This array is indexed by the connection descriptor.
 * More realistically, a struct array would be used to hold the state 
 * of each client; however, we're just holding an int (a guessed num). 
 * A 0 entry indicates currently unused fd.
 */
int target[FD_SETSIZE]; 

int main (int argc, char **argv) {

    int sock, fd;
    int client_len;
    struct sockaddr_in server, client;

    /* These are the socket sets used by select().
     * 5.7rc3 fd_set is a struct containing:
     * unsigned long fds_bits[__FD_SETSIZE / (8 * sizeof(long))];
     */
    fd_set test_set, ready_set;

    printf ("FD_SETSIZE=%d\n", FD_SETSIZE);
    
    /* Mark all entries in the target table as free. */
    for (fd=0; fd<FD_SETSIZE; fd++) target[fd] = 0;

    /* Set up the address struct for upcoming bind() */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(GUESSING_PORT);

    /* Attempt to bind() to all available network interfaces
     * (INADDR_ANY)
     */
    assertf((bind(sock, (struct sockaddr *)&server, sizeof(server))
    			== 0), "failed bind()");

    listen(sock, 5);

    /* Initially, the "test set" has in it just the server socket() 
     * descriptor.
     */
    FD_ZERO(&test_set);
    FD_SET(sock, &test_set);
    printf("server ready\n");

    /* Here is the head of the main service loop */
    while (1) {

       /* Because select() overwrites the descriptor set, we
        * must not use our "permenent" set here, we make a copy
        * instead. ready_set is the one that select() will modify.
        */
        memcpy(&ready_set, &test_set, sizeof(test_set));  
      
        /* Block on select() call until one (or more) of the fds
         * becomes ready.
         */
        select(FD_SETSIZE, &ready_set, NULL, NULL, NULL);

        /* Did we get a new connection request? If so, we accept it,
         * add the new desciptor into the read set, choose a random
         * for this client to guess.
         * Note: The only possiblity of a fd being ready on the first
         * time through the loop is the server fd being ready. A server
         * socket is considered ready for reading if there is a pending 
         * connection which can be accepted. 
         */
 
         /* FD_ISSET checks that a fd is a member (set bit) of the set */
         if (FD_ISSET(sock, &ready_set)) {
            client_len = sizeof(client);

            /* accept() blocks waiting for a connection */
            fd = accept(sock, (struct sockaddr *)&client, &client_len);
            FD_SET(fd, &test_set);
            printf("new connection on fd %d\n", fd);
            target[fd] = 1 + rand()%1000;
            printf ("answer = %d\n", target[fd]);
         }

         /* Now check all other fds for readiness */
         for (fd=0; fd < FD_SETSIZE; fd++) {
             
            /* omit the server sock descriptor */
            if (fd == sock) continue;
             
            /* Ignore fds that aren't in use */
            if (target[fd] == 0) continue; 
  
            if (FD_ISSET(fd, &ready_set)) {

               /* Get and process a guess from the client */
               if (process_request(fd) < 0) {

                  /* If the client has closed its end of the connection,
                   * or if the client guesses correctly, we close our 
                   * end, and remove the descriptor from the read set.
                   */

                   /* Close the fd so that we can reuse it */
                   close(fd);

                   /* Take fd out of the test set */
                   FD_CLR(fd, &test_set);

                   /* Mark this fd as unused in the target table */
                   target[fd] = 0;
                   printf("closing fd=%d\n", fd);
                }
             }  
          }
   }
       
} /* end main() */

int process_request(int fd) {
   int guess;
   char inbuffer[100], *p;
   p = inbuffer;

   /* Read a guess from the client */
   if (read(fd, inbuffer, 100) <= 0)
      return -1;
   guess = atoi(inbuffer);
   printf("received guess %d on fd=%d\n", guess, fd);

   /* Return 0 to indicate dialogue with this client hasn't 
    * finished yet.
    */
   if (guess > target[fd]) {
      write(fd, "guess too small\n", 16);
      return 0;
   }
   if (guess < target[fd]) {
      write(fd, "guess too large\n", 16);
      return 0;
   }
   write(fd, "correct guess!\n", 15);

   /* Return -1 to indicate dialogue with this client has 
    * completed.
    */   
   return -1;
} /* end process_request() */
