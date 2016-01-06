CC=gcc
OPCOES_COMPILADOR=-I. -I/usr/include/SOIL
TODOS_ARQUIVOS_PONTOH =
TODOS_ARQUIVOS_OBJ = main.o

%.o: ../%.c $(TODOS_ARQUIVOS_PONTOH)
	$(CC) -o $@ -c $< $(OPCOES_COMPILADOR)

all: $(TODOS_ARQUIVOS_OBJ)
	gcc -o main $^ $(OPCOES_COMPILADOR) -lSOIL -lglut -lGL

run: all
	(cd .. && exec Makefile/main)


clean:
	rm *.o main
