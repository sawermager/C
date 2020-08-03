#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int atoi (const char *);
char * reverse(char *);
char * reverse(char *s) {
   int len = 0;
   char ch, *begin, *end, *mystr, *tmp;
   int i = 0;

   len = strlen(s);
   mystr = (char *) malloc(sizeof(char) * len + 1);
   tmp = mystr;
   end = s;
   end = end + len - 1;
   while (end >= s)  {
       *mystr++ = *end--;
      
   }
   *mystr = '\0';
   return tmp;
}

   
int atoi (const char *s) {
   int sign = 1;
   unsigned int num = 0;

   do {
       if (*s == '-') {
           sign *= -1;
       }
       else if (*s >= '0' && *s <= '9') {
               num = (num *10) + (*s - '0');
       }
       else if (num > 0) {
           break;
       }
    } while (*s++);
    return (num * sign);
}

int main() {
    char *s1 = "44331";
    char *s2 = "-44331";
    char *s3 = "4A4331";
    char s[100] = "scott";
    char *a; 

    printf("%d %d %d\n", atoi(s1), atoi(s2), atoi(s3));
    reverse(s);
    printf("reverse string = %s\n", reverse(s));
}