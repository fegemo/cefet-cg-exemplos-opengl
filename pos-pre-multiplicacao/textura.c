#include <GL/glew.h>
#include <SOIL/SOIL.h>
#include <stdio.h>

GLuint carregaTextura(const char* caminho)
{
    GLuint idTextura = SOIL_load_OGL_texture(
                           caminho,
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );

    if (idTextura == 0) {
        printf("Erro do SOIL ao carregar arquivo '%s': %s\n", caminho, SOIL_last_result());
    }

    return idTextura;
}

