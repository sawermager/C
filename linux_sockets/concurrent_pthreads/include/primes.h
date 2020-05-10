
#define PRIME_PORT 61000

/* format of packet we're sending from client to each server */
struct subrange {
   int min;
   int max;
};

int isprime (int);
int count_primes(int, int);
void *func(void *);


