#include <SOIL/SOIL.h>
#include <stdio.h>

unsigned int carregaTextura(const char* caminho) {

    unsigned int idTextura = SOIL_load_OGL_texture(
        caminho,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );

    if (idTextura == 0) {
        printf("Erro ao carregar textura com caminho '%s': '%s'\n", caminho, SOIL_last_result());
    }
}
