CC = gcc
CFLAGS = -Wall -Wextra -g
LDLIBS = -lm

all: rotas2024

rotas2024: functions.o main.o
	clear
	@echo "Compiling and running the program..."
	$(CC) $(CFLAGS) -o rotas2024 functions.o main.o $(LDLIBS)
	@echo "Compiled successfully!"

functions.o: functions.c functions.h
	$(CC) $(CFLAGS) -c functions.c

main.o: main.c functions.h
	$(CC) $(CFLAGS) -c main.c

clean:
	@rm -f rotas2024 *.o

valgrind: rotas2024
	valgrind --leak-check=full --track-origins=yes ./rotas2024 LIS FRA -L 1 -TC -D





