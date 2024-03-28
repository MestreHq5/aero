CC = gcc
CFLAGS = -Wall -Wextra -g

all: program

program: functions.o main.o
	clear
	@echo "Compiling and running the program..."
	$(CC) $(CFLAGS) -o program functions.o main.o
	@echo "Compiled, executing..."
	./program

functions.o: functions.c functions.h
	$(CC) $(CFLAGS) -c functions.c

main.o: main.c functions.h
	$(CC) $(CFLAGS) -c main.c

clean:
	@rm -f program *.o

