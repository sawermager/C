
// Using dynamic programming 
#include <stdio.h>


int fib_norecursion(int num);
int fib_recursion(int num);
void init(int num);

#define NIL -1
#define MAX 100
int lookup[MAX];

int main() {

   int i, num;
  
   printf("Enter num\n");
   scanf("%d", &num);

   init(num);
   for (i=0; i<=num; ++i) {
     printf ("fib_dynam_norecur(%d) = %d\n", i, fib_norecursion(i));
     printf ("fib_dynam_recur(%d) = %d\n", i, fib_recursion(i));
   }
}


int fib_norecursion(int num) {
   int i, f[num];
   f[0] = 0;
   f[1] = 1;

   for (i=2; i<=num; ++i) {
      f[i] = f[i - 1] + f[i - 2];
   }
   return f[num];
}

int fib_recursion(int num) {

  if (lookup[num] == NIL) {
     if (num <= 1) {
        lookup[num] = num;
     }
     else 
        lookup[num] = fib_recursion(num - 1) + fib_recursion(num - 2);
  }
  return lookup[num];
}

void init(int num) {
  int i;
  for (i=0; i<=num; ++i) {
    lookup[i] = NIL;
  }
}

