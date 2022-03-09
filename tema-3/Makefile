# Ciobanu Andrei-Bogdan 312 CB

CC = gcc
CFLAGS = -Wall -Wextra -g

SRCS_DIR = src

OBJS = cJSON.o utils.o solver.o main.o

EXE = sudoku

.PHONY: clean

run: build
	./$(EXE)

build: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXE)

cJSON.o: $(SRCS_DIR)/cJSON.c
	$(CC) $(CFLAGS) $(SRCS_DIR)/cJSON.c -c

utils.o: $(SRCS_DIR)/utils.c
	$(CC) $(CFLAGS) $(SRCS_DIR)/utils.c -c

solver.o: $(SRCS_DIR)/solver.c
	$(CC) $(CFLAGS) $(SRCS_DIR)/solver.c -c

main.o: $(SRCS_DIR)/main.c
	$(CC) $(CFLAGS) $(SRCS_DIR)/main.c -c

clean:
	rm *.o
