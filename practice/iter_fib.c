
#include <stdlib.h>
#include <stdio.h>


int fib(int);

int main() {

  int i=1, num;
  
  printf("enter nth term to find fib sequence\n");
  scanf("%d", &num);

  while (i <= num) {
      printf ("term: %d: fib: %d\n", i, fib(i++));
  }
  return EXIT_SUCCESS;
}

int fib(int num) {
   int i, t1 = 0, t2 = 1, nextterm;
   for (i=0; i< num; ++i) {
       nextterm = t1 + t2;
       t1 = t2;
       t2 = nextterm;
   }
   return nextterm;
}