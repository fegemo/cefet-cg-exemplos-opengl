CC=g++
OPCOES_COMPILADOR=-I. -lglut -lGLU -lGLEW -lGL
TODOS_ARQUIVOS_PONTOH = ../error.h ../shader.h
TODOS_ARQUIVOS_OBJ = shader.o main.o

# main.o: ../main.cpp $(TODOS_ARQUIVOS_PONTOH)
# 	$(CC) -o $@ -c $< $(OPCOES_COMPILADOR)

%.o: ../%.cpp $(TODOS_ARQUIVOS_PONTOH)
	$(CC) -o $@ -c $< $(OPCOES_COMPILADOR)

all: $(TODOS_ARQUIVOS_OBJ)
	$(CC) -o main $^ $(OPCOES_COMPILADOR)

run: export vblank_mode=0
run: export __GL_SYNC_TO_VBLANK=0
run: all
	(cd .. && exec Makefile/main && cd Makefile)

clean:
	rm *.o main
