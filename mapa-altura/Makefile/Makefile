CC= gcc
TODOS_ARQUIVOS_PONTOH = ../vetor.h
TODOS_ARQUIVOS_OBJ = main.o vetor.o
OPCOES_COMPILADOR = -Wall -std=c99

%.o: ../%.c $(TODOS_ARQUIVOS_PONTOH)
	$(CC) -o $@ -c $< $(OPCOES_COMPILADOR)

all: $(TODOS_ARQUIVOS_OBJ)
	$(CC) -o main $^ $(OPCOES_COMPILADOR) -lSOIL -lglut -lGLU -lGL -lm

run: all
	(cd .. && exec Makefile/main)

clean:
	rm *.o main
