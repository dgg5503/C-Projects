CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror -pedantic -g

DissectPackets: dissectPackets.o
	$(CC) $(CFLAGS) -o dissectPackets dissectPackets.o

dissectPackets.o : dissectPackets.c utils.h dissectPackets.h
	$(CC) $(CFLAGS) -c dissectPackets.c
