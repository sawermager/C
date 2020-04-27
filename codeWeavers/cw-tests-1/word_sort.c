#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include "word_sort.h"

// struct used to pass around debug info and dst_len
typedef struct {
   unsigned int src_len;
   unsigned int word_cnt;
   unsigned int total_char_cnt;
   unsigned int dst_len;
} dstinfo_struct;

// fn to get src and dst info
dstinfo_struct get_src_info (const char *);

// fn to sort alphabetically
int mystrcmp (const void *a, const void *b);

// fn to sort reverse alphabetically
int mystrcmp_reverse (const void *a, const void *b);

// fn to sort alphabetically ignoring case
int mystrcmp_ignore_case (const void *a, const void *b);

// fn to print results output as specified in API spec example
void print_results (const char *, unsigned it, char *, unsigned int);

// fn to allow printing results output of any special chars in src
char * str_escape_special_chars(const char *, unsigned int);


// fm main() for testing word_sort()
int main (int argc, char **argv) {
   
   const char *src = "The   \t\n cat  sat";
   char *dst = NULL;
   dstinfo_struct dst_struct;
   unsigned int retval = 0;
   unsigned int flags = WORD_SORT_REVERSE;
   pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
   
   // get len of dst string which include null terminator to string
   dst_struct = get_src_info(src);

   // Allocate space for dst buffer. Include space for appending a NUL terminator
   // NULL malloc return is checked in word_sort() per API spec
   dst = (char *) malloc (sizeof(char) * dst_struct.dst_len + 1);
  
   // Applying 'thread safe' to fn (I would ideally make the data thread-safe instead)
   pthread_mutex_lock(&mutex);
   retval = word_sort (src, dst, dst_struct.dst_len, flags); 
   pthread_mutex_unlock(&mutex);

   // Output results
   print_results (src, flags, dst, retval);

} // end main()


// Print results output from word_sort()
void print_results (const char *src, unsigned int flags, char *dst, unsigned int retval) {

   char *flag_string = NULL;
   char *src_string = NULL;
 
   if (NULL == (src_string = (char *) malloc (sizeof(char) * strlen(src))))
      assert ((NULL != src_string) && "Failed to allocate memory for src_string");

   // Make flags printable as string for results output
   if (NULL == (flag_string = (char *) malloc (sizeof(char) * 32)))
      assert ((NULL != flag_string) && "Failed to allocate memory for flag_string");
   switch (flags) {
      case 0:
         strcpy (flag_string, "0");
         break;
      case WORD_SORT_IGNORE_CASE:
         strcpy (flag_string, "WORD_SORT_IGNORE_CASE");
         break;
      case WORD_SORT_REVERSE:
         strcpy (flag_string, "WORD_SORT_REVERSE");
         break;
      default:
         strcpy (flag_string, "INVALID");
   }

   //Handle special chars in src for results output 
   src_string = str_escape_special_chars (src, strlen(src)); 

   printf("%-30s %-20s %-20s %-20s\n", "src", "flags", "dst", "retval");
   printf("===============================================================================\n");
   printf("%-30s %-20s %-20s %-20u\n", src_string, flag_string, dst, retval);

   free (flag_string);
   flag_string = NULL;
   free (src_string);
   src_string = NULL;

} // end print_results()


// Handle printing of special characters in src
char * str_escape_special_chars (const char *src, unsigned int src_len) {

    char chr[3];
    char *buffer_str = NULL;
    unsigned int len = 1, blk_size;
    char *ptr_src_tmp = NULL;

    if (NULL == (buffer_str = (char *) malloc (sizeof(char) * 1024)))
       assert ((NULL != buffer_str) && "Falied to allocate space for buffer_str");

    ptr_src_tmp = (char *) src;
    while (ptr_src_tmp < src + src_len) {
        blk_size = 2;
        switch (*ptr_src_tmp) {
            case '\n':
                strcpy(chr, "\\n");
                break;
            case '\t':
                strcpy(chr, "\\t");
                break;
            case '\v':
                strcpy(chr, "\\v");
                break;
            case '\f':
                strcpy(chr, "\\f");
                break;
            case '\a':
                strcpy(chr, "\\a");
                break;
            case '\b':
                strcpy(chr, "\\b");
                break;
            case '\r':
                strcpy(chr, "\\r");
                break;
            default:
                sprintf(chr, "%c", *ptr_src_tmp);
                blk_size = 1;
                break;
        }
        strcat(buffer_str, chr);
        ++ptr_src_tmp;
    }
    return buffer_str;
}


/*****************************************************************************************************************
NAME
        word_sort

SYNOPSIS
        #include "word_sort.h"

        unsigned int word_sort( const char *src, char *dst, unsigned int dst_len, unsigned int flags );

DESCRIPTION
        The word_sort() function sorts a whitespace separated list of words in the string src into
        alphabetical order and stores them separated by a single space in the provided output buffer dst.
        Words are read from src until a '\0' is encountered.  A '\0' appended to dst.  The dst_len parameter
        is the size of the provided output buffer dst in chars.  Note that the required output buffer
        length may be less than strlen(src) + 1, if src contains adjacent white spaces.  If src contains
        only whitespace the output buffer will contain a single '\0'.

        The flags parameter maybe contain the following:

        WORD_SORT_REVERSE
                Sort in reverse alphabetical order.

        WORD_SORT_IGNORE_CASE
                Case insensitive sort.

RETURN VALUE
       The function returns the number of characters written to dst including the trailing '\0'.
       If there is insuffient space in dst then the function returns 0.

       >>>>> NOTE: The number of characters written to dst does NOT include the white spaces added, ONLY alphanumerics.

THREAD SAFETY
       The function is thread safe.

EXAMPLES
        src                       flags                             dst             return value
        ========================================================================================
        "The   \t\n cat  sat"     0                                 "The cat sat"   12
        "The   \t\n cat  sat"     WORD_SORT_IGNORE_CASE             "cat sat The"   12
        "The   \t\n cat  sat"     WORD_SORT_REVERSE                 "sat cat The"   12
*****************************************************************************************************************/


unsigned int word_sort( const char *src, char *dst, unsigned int dst_len, 
                        unsigned int flags ) {
   
   int i = 0;
   int j = 0;
   int MAX_WORDS = 1024;
   char *src_tmp = NULL;
   char *ptr_src_tmp = NULL;
   char *ptr_dst_tmp = NULL;
   char **dst_sort = NULL;
   char *ptr_orig_dst = NULL;
   unsigned int total_char_cnt = 0;
   unsigned int loop_char_cnt = 0;
   unsigned int word_cnt = 0;
   size_t src_tmp_len = 0;
   
   // Unable to allocate enough space dst. Space for dst was allocated first in main().
   if (dst == NULL) return 0;
 
   // Unable to allocate other dst buffers for sorting
   if (NULL == (dst_sort = (char **) malloc (sizeof(char *) * MAX_WORDS)))
      return 0;

   // Validity check of other word_sort() args 
   assert ((flags == 0) || (flags == WORD_SORT_REVERSE) || (flags == WORD_SORT_IGNORE_CASE));
   assert (src != NULL);

   if (NULL == (src_tmp = (char *) malloc (sizeof(char) * strlen(src) + 1)))
       assert ((NULL != src_tmp) && "Failed to allocate spce for src_tmp");

   // Add a NUL terminator to src (not clear why this is needed but will add it
   // because of following API routine functionality requirement:
   // "Words are read from src until a '\0' is encountered."
   // strcpy appends a NUL terminator src_tmp in the copy automatically

   strcpy (src_tmp, src);

   // Build dst string (NUL terminated) from src string
   // strlen doesn't include NUL terminator

   ptr_orig_dst = dst;
   src_tmp_len = strlen(src_tmp);
   ptr_src_tmp = src_tmp;
   while (*ptr_src_tmp != '\0') {
      if (isalnum(*ptr_src_tmp)) {
         loop_char_cnt = 0;
         ptr_dst_tmp = dst;
         while ((isalnum(*ptr_src_tmp) && (ptr_src_tmp < src_tmp + src_tmp_len))) {
            *dst = *ptr_src_tmp;
            total_char_cnt++;
            loop_char_cnt++;
            dst++;
            ptr_src_tmp++;
         }      

         // Add a space between each word in the dst buffer
         *dst = ' ';

         // We now have a full word, add it into 2d array for sorting later
         if (NULL == (*(dst_sort + word_cnt) = (char *) malloc (sizeof(char) * loop_char_cnt)))
            assert ((NULL == *(dst_sort + i)) && "Failed to allocate space for dst_sort");
         for (i=0; i<loop_char_cnt; i++) {
            *(*(dst_sort + word_cnt) + i) = *(ptr_dst_tmp + i);
         }
         word_cnt++;
         dst++;
      } else {
         ptr_src_tmp++; 
      }
   }

   // Append NUL terminator to our dst buffer
   *dst = '\0';
   total_char_cnt++;

   // Sort based on flags value 
   if (flags == 0) {
      qsort (dst_sort, word_cnt, sizeof(char *), mystrcmp);
   } else if (flags == WORD_SORT_REVERSE) {
      qsort (dst_sort, word_cnt, sizeof(char *), mystrcmp_reverse);
   } else if (flags == WORD_SORT_IGNORE_CASE) {
      qsort (dst_sort, word_cnt, sizeof(char *), mystrcmp_ignore_case);
   }
   for (i=0;i<word_cnt;i++)

   // Char array, dst_sort, is sorted, now copy it into dst buffer with required white spaces
   dst = ptr_orig_dst;
   for (i=0; i < word_cnt; i++) {
      for (j=0; j<strlen(*(dst_sort + i)); j++) {
         *(dst + j) = *(*(dst_sort + i) + j);
      }
      dst += strlen(*(dst_sort + i));
      if (i != word_cnt -1) {
         *dst = ' ';
         dst++;
      } else 
         *dst = '\0';
   }
    
   // Free up dynamic mem
   free (src_tmp);
   for (i=0; i < word_cnt; i++)
      free (*(dst_sort + i));
      *(dst_sort + i) = NULL;
   src_tmp = NULL;
   return total_char_cnt;
} // end word_sort() 


// Get src, dst, and handy debug info
dstinfo_struct get_src_info (const char *src) {
   dstinfo_struct dst_info;
   dst_info.src_len = strlen(src);

   // Find number of words in src char array
   const char *ptr = src;
   while (ptr < (src+dst_info.src_len)) {
      if (isalnum(*ptr)) {
         while ((isalnum(*ptr) && (ptr < src+dst_info.src_len))) {
            ptr++;
            (dst_info.total_char_cnt)++;
         }      
         (dst_info.word_cnt)++;
      }
      else ptr++;
   }
   dst_info.dst_len = (dst_info.word_cnt - 1) + dst_info.total_char_cnt + 1;
   return dst_info;
} // end get_src_info()

// Sort char array in alphabetical order
int mystrcmp (const void *a, const void *b) {
   const char *aa = *(const char **)a;
   const char *bb = *(const char **)b;
   return strcmp(aa, bb);
} // end mystrcmp()

// Sort char array in reverse alphabetical order
int mystrcmp_reverse (const void *a, const void *b) {
   const char *aa = *(const char **)a;
   const char *bb = *(const char **)b;
   return strcmp(bb, aa);
} // end mystrcmp_reverse()

// Sort char array in alphabetical order ignoring case
int mystrcmp_ignore_case (const void *a, const void *b) {
   const char *aa = *(const char **)a;
   const char *bb = *(const char **)b;
   return strcasecmp(aa, bb);
} // end mystrcmp_ignore_case()
