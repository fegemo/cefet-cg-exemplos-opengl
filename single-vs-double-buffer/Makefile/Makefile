CC=g++
OPCOES_COMPILADOR=-I. -lglut -lGLU -lGLEW -lGL
TODOS_ARQUIVOS_PONTOH =
TODOS_ARQUIVOS_OBJ = main.o

%.o: ../%.c $(TODOS_ARQUIVOS_PONTOH)
	$(CC) -o $@ -c $< $(OPCOES_COMPILADOR)

all: $(TODOS_ARQUIVOS_OBJ)
	gcc -o main $^ $(OPCOES_COMPILADOR)

run: export vblank_mode=0
run: export __GL_SYNC_TO_VBLANK=0
run: all
	(cd .. && exec Makefile/main && cd Makefile)

clean:
	rm *.o main
