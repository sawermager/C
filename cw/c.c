
#include <stdio.h>
#include <ctype.h>
void  main() {

	char *x="scott here";
	while (*x != '\0') {
	   printf ("%d\n", isalnum(*x));
	   x++;
	}
}
