CC=gcc
OPCOES_COMPILADOR=-I. -lSOIL -lglut -lGLU -lGLEW -lGL
TODOS_ARQUIVOS_OBJ = carregaTextura.o main.o

%.o: ../%.c ../*.h
	$(CC) -o $@ -c $< $(OPCOES_COMPILADOR)

all: $(TODOS_ARQUIVOS_OBJ)
	gcc -o main $^ $(OPCOES_COMPILADOR)

run: all
	cd .. && ./Makefile/main

clean:
	rm *.o main
