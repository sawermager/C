#include <stdlib.h>
#include <stdio.h>

int sum(int num);
int sum(int num) {
   int m, sum=0;
   while (num > 0) {
       m = num%10;
       sum += m;
       printf("m = %d num=%d m=%d\n", m, num, sum);
       num = num/10;
   }
   return sum;
}

int main () {
    int num;
    printf("Enter num:-\t");
    scanf("%d", &num);

    printf("sum = %d\n", sum(num));
}


