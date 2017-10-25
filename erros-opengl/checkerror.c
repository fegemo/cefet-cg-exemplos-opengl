#include <GL/glew.h>
#include <stdio.h>
#include <string.h>

void check(const char *file, int line) {
    GLenum error;

    while ((error = glGetError()) != GL_NO_ERROR) {
        char errorName[100];

        switch(error) {
            case GL_INVALID_OPERATION:
                strcpy(errorName, "INVALID_OPERATION");
                break;

            case GL_INVALID_ENUM:
                strcpy(errorName, "INVALID_ENUM");
                break;

            case GL_INVALID_VALUE:
                strcpy(errorName, "INVALID_VALUE");
                break;

            case GL_OUT_OF_MEMORY:
                strcpy(errorName, "OUT_OF_MEMORY");
                break;

            case GL_INVALID_FRAMEBUFFER_OPERATION:
                strcpy(errorName, "INVALID_FRAMEBUFFER_OPERATION");
                break;
        }


        fprintf(stderr, "%s (code: GL_%s)- %s: %d\n", gluErrorString(error), errorName, file, line);
    }
}
