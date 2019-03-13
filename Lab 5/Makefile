CC=gcc
DEPS = automata.h map.h nier.h threads.h
CFLAGS = -Wall -std=c99 -pthread -Wno-missing-braces
OBJ = lab5.o automata.o threads.o nier.o map.o

default: main

# linking
main: $(OBJ)
	$(CC) $(CFLAGS) -o lab5 $(OBJ)

# compiling
lab5.o: lab5.c automata. map.h nier.h threads.h
	$(CC) $(CFLAGS) -c lab5.c

automata.o: automata.h
map.o: map.h
nier.o: nier.h
threads.o: threads.h 

clean:
	-rm -f *.o
	-rm main
