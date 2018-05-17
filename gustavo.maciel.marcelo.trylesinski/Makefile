CC     = gcc
CFLAGS = -std=c99 -Wall -pedantic -O3 -flto

main: main.o mtrio.o mtrmul.o mtrmem.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c
	$(CC) $(CFLAGS) -c $<

mtrio.o: mtrio.c mtrio.h
	$(CC) $(CFLAGS) -c $<

mtrmul.o: mtrmul.c mtrmul.h
	$(CC) $(CFLAGS) -c $<

mtrmem.o: mtrmem.c mtrmem.h
	$(CC) $(CFLAGS) -c $<

teste: test.o mtrio.o mtrmul.o mtrmem.o
	$(CC) $(CFLAGS) -o $@ $^

test.o: test/test.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o *~ main teste
