#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "primes.h"
#include "assertf.h"

int isprime (int n) {
   int i;
   for (i=2; i*i <= n; i++) 
      if (n%i == 0) return 0;
   return 1;
}

int count_primes (int min, int max) {
   int i, count=0;
   for (i=min; i<max; i++) 
      count += isprime(i);
   printf ("range %8d - %8d : count = %d\n", min, max, count);
   return count;
}

void main () {
   int sock, fd, client_len, count;
   struct sockaddr_in server, client;
   struct subrange limits;

   sock = socket(AF_INET, SOCK_STREAM, 0);
   if (sock < 0) 
      assertf(sock > 0, "failed socket()");
   server.sin_family = AF_INET;
   server.sin_addr.s_addr = htonl(INADDR_ANY);
   server.sin_port = htons(PRIME_PORT);
   assertf((bind(sock, (struct sockaddr *)&server, sizeof(server)) == 0),
            "failed bind()");

   listen(sock, 5);
   printf("listening on port %d ...\n", PRIME_PORT);
   
   while (1) {
      client_len = sizeof(client);
      fd = accept(sock, (struct sockaddr *)&client, &client_len);
      assertf(fd > 0, "failed accept()");
      
      printf("got connection\n");
      while (1) {
         if (read(fd, &limits, sizeof(limits)) != sizeof(limits)) 

            /* assume EoF and drop the connection */
            break;    
         count = count_primes(limits.min, limits.max);
         write(fd, &count, sizeof(count));
      }
      close(fd);
    }
} /* end main() */
