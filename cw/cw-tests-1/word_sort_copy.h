unsigned int word_sort( const char *src, char *dst, unsigned int dst_len, unsigned int flags );
unsigned int get_src_info(const char *src);
int mystrcmp(const void *aIn, const void *bIn , void *thunkIn);

#define WORD_SORT_REVERSE       0x1
#define WORD_SORT_IGNORE_CASE   0x2
