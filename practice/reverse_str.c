#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *reverse_str(char *);

int main() {

    char str[] = "scott was here";
    printf ("string = %s\n", str);
    printf ("reverse_string = %s\n", reverse_str(str));
    return EXIT_SUCCESS;
}


char *reverse_str(char *str) {
   char *ptr_str_start, *ptr_str_end, ch;
   int i;

   // Set the start and end ptrs
   ptr_str_start = str;
   ptr_str_end = str + (strlen(str)-1);
   for (i=0; i<(strlen(str)/2); i++) {
      ch = *str;
      *str = *ptr_str_end;
      *ptr_str_end =  ch;
      str++;
      ptr_str_end--;
   }
   return str;

}