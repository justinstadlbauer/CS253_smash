CC = gcc
CFLAGS = -Wall -g -O0
EXE1 = smash
EXE2 = smash_v1
all:	$(EXE1) $(EXE2)

$(EXE1): smash.c
	$(CC) $(CFLAGS) -o $@ $^

$(EXE2): smash_v1.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o $(EXE1) $(EXE2)
