CC= g++
OPCOES_COMPILADOR= -I/usr/include/SOIL
TODOS_ARQUIVOS_PONTOH =
TODOS_ARQUIVOS_OBJ = main.o

%.o: ../%.cpp $(TODOS_ARQUIVOS_PONTOH)
	$(CC) -o $@ -c $< $(OPCOES_COMPILADOR)

all: $(TODOS_ARQUIVOS_OBJ)
	$(CC) -o main $^ $(OPCOES_COMPILADOR) -lglut -lGL -lGLU -lSOIL

run: all
	(cd .. && exec Makefile/main)

clean:
	rm *.o main
