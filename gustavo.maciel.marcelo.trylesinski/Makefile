CC     = gcc
CFLAGS = -std=c99 -Wall -pedantic -O3 -flto

main: main.o matrixio.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c matrixio.c matrixio.h
	$(CC) $(CFLAGS) -c $<

matrixio.o: matrixio.c matrixio.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o main 
