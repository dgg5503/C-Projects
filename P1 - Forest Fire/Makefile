SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))
PROG = wildfire

CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -g
CCFLAGS = -std=c99 -Wall -Wextra -pedantic -g
CXXFLAGS = -std=c99 -Wall -Wextra -pedantic -g

$(PROG) : $(OBJ)
	$(CC) $(CCFLAGS) $(OBJ) -o $(PROG)
 
$(OBJ) : $(SRC)

clean:
	rm $(PROG)
	rm $(OBJ)
	