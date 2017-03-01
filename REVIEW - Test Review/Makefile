CFLAGS = -ggdb -Wall -Wextra -pedantic -std=c99 
CLIBFLAGS = -lpthread -lm
CC = gcc

main: main.o file1.o file2.o
	$(CC) $(CFLAGS) -o main main.o file1.o file2.o $(CLIBFLAGS)

main.o: file1.o file2.o main.h
	$(CC) $(CFLAGS) -c main.c

file1.o: file1.c file1.h
	$(CC) $(CFLAGS) -c file1.c

file2.o: file2.c file2.h
	$(CC) $(CFLAGS) -c file2.c

clean:
	rm main.o
	rm file1.o
	rm file2.o
	rm main
	clear
