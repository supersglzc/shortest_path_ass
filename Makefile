C = gcc
CFLAGS = -Wall -std=c99

all: test

graph.o: graph.c graph.h 
	$(CC) $(CFLAGS) -c graph.c

test.o: test.c graph.o graph.h
	$(CC) $(CFLAGS) -c test.c
	
test: test.o 
	$(CC) $(CFLAGS) -o test graph.o test.o 

clean:
	rm *.o test
