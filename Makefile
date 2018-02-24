CC = gcc
CFLAGS = -Wall -g
all:

smash: smash.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o smash 
