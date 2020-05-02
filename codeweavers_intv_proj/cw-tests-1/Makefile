
OBJECTS = word_sort_copy.o m_cw_test.o

m_cw_test.o: m_cw_test.c
	gcc -o $@ -c $<
	
word_sort.o: word_sort_copy.c word_sort_copy.h
	gcc -g3 -o $@ -c $<  

word_sort_copy: word_sort_copy.o
	gcc -g3 -o $@ $<

all: $(objects) word_sort_copy

clean:
	rm -rf *.o word_sort_copy



