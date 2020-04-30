
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "word_sort_copy.h"
#include <assert.h>

#include <unistd.h>

int main (int argc, char **argv) {
 
 const char *src = "The   \t\n cat  sat";
 char *dst = NULL;
 unsigned int dst_len = 0;
 long flags = 0;
 int word_sort_retval = 0;

 
 if (argc == 2) flags = strtol(argv[1], NULL, 0);
 printf ("flag = 0x%lx\n", flags);

 /* Determine size, dst_len, of the dst buffer and allocate buffer */
 dst_len = get_src_info(src);

 /* To make word_sort() threadsafe, the caller should be responsible for allocating space */
dst = (char *) malloc (dst_len);

/* Call word_sort() */
word_sort_retval = word_sort(src, dst, dst_len, flags);



} /* end main() */


/**
 * @brief My implementation of the project described in word_test.txt
 * 
 * @param src  - string
 * @param dst  - destination string of alphabetically sorted words in 'src' seperated by whitespace
 * @param dst_len - size of dst buffer (words are read from src until '\0', NUL terminator is reached)
 * Note that the required output buffer length may be less than strlen(src) + 1, if src contains adjacent 
 * white spaces.  If src contains only whitespace the output buffer will contain a single '\0'.
 * @param flags - WORD_SORT_REVERSE | WORD_SORT_IGNORE_CASE
 * @return unsigned int 
 */
unsigned int word_sort( const char *src, char *dst, unsigned int dst_len, unsigned int flags ) {

   int retval = 0;
   char *buffer_tmp = NULL;
   const char *tmp_src = NULL;
   char *tmp_dst = NULL;

   /* Check valid args */
   assert (src != NULL);
   assert ((flags == 0) || (flags == WORD_SORT_REVERSE) || (flags == WORD_SORT_IGNORE_CASE));
   if (dst == NULL) return 0; /* per requirement: return 0 if dst isn't sufficient size */

   tmp_src = src;
   tmp_dst = dst;
   while (*tmp_src) {
      while (isspace(*tmp_src))  tmp_src++;
      while (isalnum(*tmp_src)) {
         *tmp_dst = *tmp_src;
         tmp_dst++;
         tmp_src++;
      }
      *tmp_dst = ' ';
      if (!(*tmp_src)) {
          *tmp_dst = *tmp_src;
          break;
      }
      else tmp_dst++;
    }

    /* Sort dst based on flags */

return (strlen(dst) + 1);
}

/**
 * @brief Determine, from src, the length neede for dst buffer
 * 
 * @param src - string of word(s)
 * @return int - size of dst buffer needed
 */
unsigned int get_src_info(const char *src) {
    unsigned int retval = 0;
    const char *ptr = NULL;
    int char_cnt = 0;
    int word_cnt = 0;

    /* Count relevent chars in each word in src string reading until NUL encountered*/
    ptr=src;
    while (*ptr) {
        while (isspace(*ptr))  ptr++;
        while ((isalnum(*ptr)) && !(isspace(*ptr)) && *ptr) {
            char_cnt++;
            ptr++;
        }
        word_cnt++;
    }
    /* If word_cnt == 0, then still need buffer size to allow empty string */
    if (word_cnt ==0) {
        retval = sizeof(char);
    }
    else {
        /** This is total alpha character counted + Number of spaces needed between words
        *   + 1 for NUL terminator
        */
        retval = char_cnt + (word_cnt -1) + sizeof(char);
    }
    return retval;
}
