CC = gcc
CFLAGS = -Wall -g -O0
EXE1 = smash

all:	$(EXE1)

$(EXE1): smash.c history.c history.h
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o *~ $(EXE1) fork_ex
