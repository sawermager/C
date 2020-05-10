
/* multi-threaded client to count prime
 * numbers using multiple servers.
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include "assertf.h"
#include "primes.h"

#define MAX_SERVERS 	16

/* Globals shared with all threads */
pthread_t handles[MAX_SERVERS];
int nprimes = 0;
int lower = 0;
int biggest = 10000000;
int chunk_size = 1000;

pthread_mutex_t prime_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lower_lock = PTHREAD_MUTEX_INITIALIZER;

/* Fn the threads will all run */
void *func(void *s) {
   struct subrange range;
   int fd, n;

   /* Get this thread's file descriptor to the server */
   fd = *(int *)s;
   
   /* Send ranges to our server until there are none left */
   while (1) {
      pthread_mutex_lock(&lower_lock);
      range.min = lower;
      range.max = lower + chunk_size;
      lower += chunk_size;
      pthread_mutex_unlock(&lower_lock);
      if (range.min >= biggest)

         /* No more ranges */
         pthread_exit(NULL);
       
      /* Send the range to our primes server */
      write(fd, &range, sizeof(range));
      read(fd, &n, sizeof(int));
      pthread_mutex_lock(&prime_lock);
      nprimes += n;
      pthread_mutex_unlock(&prime_lock);
   }
}

int main (int argc, char **argv) {
   int i, sock, nsrv = 0;
   struct hostent *host_info;
   struct sockaddr_in server;
   int opt;

   /* Process -n an d-c command line options.
    * Unrecognized options are ignored.
    */
   while (1) {
      opt = getopt(argc, argv, "n:c:");
      if (opt == -1) break;
      switch(opt) {
         case 'n': biggest = atoi(optarg); break;
         case 'c': chunk_size = atoi(optarg); break;
         default: printf("wtf\n"); break;
      }
   }
   argc -= optind;
   argv += optind;
   printf("biggest = %d, chunk_size = %d\n", biggest, chunk_size);

   /* Loop over remaining command line arguments (hosts).
    * Attempt to resolve each host and connect.
    * Successful connections are placed in 'active' table.
    * Unsuccessful ones are reprted and ignored.
    */
   assertf(argc != 0, "no servers specified -- giving up\n");
   
   /* Loop over all potential servers on command line */
   for (i=0; i < argc; i++) {
      host_info = gethostbyname(argv[i]);
      if (host_info == NULL) {
         printf("cannot resolve %s -- ignoring\n", argv[i]);
         
         /* try next host */
         continue;
      }

      /* Connect to prime server on this host and create socket */
      server.sin_family = AF_INET;
      memcpy(&server.sin_addr, host_info->h_addr, host_info->h_length);
      server.sin_port = htons(PRIME_PORT);
      if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
         printf("cannot connect to server %s -- ignoring\n", argv[i]);

         /* try next host */
         continue;
      }
      printf("connected to %s\n", argv[i]);
      
      /* Spawn a thread to send packets to this server */
      pthread_create(&handles[nsrv], NULL, func, &sock);
      nsrv++;
   }      
   
   assertf(nsrv != 0, "no servers found -- giving up.\n");
   printf("using %d servers\n", nsrv);

   /* Wait until all the threads have run out of work */
   for (i=0; i<nsrv; i++) 
      pthread_join(handles[i], NULL);

   /* Print the answer */
   printf("found %d primes\n", nprimes);
} /* end main() */
   
      
    
      























   


