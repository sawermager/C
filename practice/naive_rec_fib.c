
#include <stdlib.h>
#include <stdio.h>

int fib(int num);

int main() {
   int num, i;

   printf("Enter nth term to determine fib(n)");
   scanf("%d", &num);

   for (i=0; i <= num; ++i) {
       printf("term: %d: fib: %d\n", i, fib(i));
   }
   return EXIT_SUCCESS;
}

int fib(int num) {
  int i, result;
  if (num <=1) return num;
  result = fib(num -1) + fib(num -2);
  return result;
}