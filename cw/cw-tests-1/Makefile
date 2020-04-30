
OBJECTS = word_sort.o m_cw_test.o

m_cw_test.o: m_cw_test.c
	gcc -o $@ -c $<
	
word_sort.o: word_sort.c word_sort.h
	gcc -o $@ -c $< 

word_sort: word_sort.o
	gcc -o $@ $<

all: $(objects) word_sort

clean:
	rm -rf *.o word_sort


