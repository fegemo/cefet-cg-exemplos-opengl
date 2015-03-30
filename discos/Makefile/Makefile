CC=gcc
CFLAGS=-I.
DEPS = 
OBJ = main.o

%.o: ../%.c $(DEPS)
	$(CC) -o $@ $< $(CFLAGS)

all: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)