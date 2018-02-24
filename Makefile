CC = gcc
CFLAGS = -Wall -g
EXE = smash
all:	$(EXE)

$(EXE): smash.o
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o smash 
