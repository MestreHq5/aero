CC = gcc
CFLAGS = -Wall -Wextra -g
LDLIBS = -lm

all: rotas2024

rotas2024: functions.o algorithms.o main.o
	clear
	@echo "Compiling and running the program..."
	$(CC) $(CFLAGS) -o rotas2024 functions.o algorithms.o main.o $(LDLIBS)
	@echo "Compiled successfully!"

functions.o: functions.c functions.h
	$(CC) $(CFLAGS) -c functions.c

algorithms.o: algorithms.c algorithms.h functions.h
	$(CC) $(CFLAGS) -c algorithms.c

main.o: main.c functions.h algorithms.h
	$(CC) $(CFLAGS) -c main.c

clean:
	@rm -f rotas2024 *.o




