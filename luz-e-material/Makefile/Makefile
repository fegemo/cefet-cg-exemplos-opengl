CC= g++
OPCOES_COMPILADOR= -I/usr/include/SOIL
TODOS_ARQUIVOS_PONTOH = -lglut -lGLU -lSOIL -lGLEW -lGL -lm
TODOS_ARQUIVOS_OBJ = main.o

%.o: ../%.cpp $(TODOS_ARQUIVOS_PONTOH)
	$(CC) -o $@ -c $< $(OPCOES_COMPILADOR)

all: $(TODOS_ARQUIVOS_OBJ)
	$(CC) -o main $^ $(OPCOES_COMPILADOR)

run: all
	(cd .. && exec Makefile/main)

clean:
	rm *.o main
