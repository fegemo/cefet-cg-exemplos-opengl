CC=gcc
OPCOES_COMPILADOR=-I. -lglut -lGLU -lGLEW -lGL -std=gnu11 -Wall
TODOS_ARQUIVOS_PONTOH =
TODOS_ARQUIVOS_OBJ = lista.o malha.o modelo.o main.o

%.o: ../%.c $(TODOS_ARQUIVOS_PONTOH)
	$(CC) -o $@ -c $< $(OPCOES_COMPILADOR)

all: $(TODOS_ARQUIVOS_OBJ)
	gcc -o main $^ $(OPCOES_COMPILADOR)

run: all
	(cd .. && exec Makefile/main)

clean:
	rm *.o main
