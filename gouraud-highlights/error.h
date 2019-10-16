#ifndef glCheckError

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string.h>
#include <stdio.h>


GLenum glCheckError_(const char *file, int line) {
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        char error[1024];
        switch (errorCode) {
        case GL_INVALID_ENUM:
            strcpy(error, "INVALID_ENUM");
            break;
        case GL_INVALID_VALUE:
            strcpy(error, "INVALID_VALUE");
            break;
        case GL_INVALID_OPERATION:
            strcpy(error, "INVALID_OPERATION");
            break;
        case GL_STACK_OVERFLOW:
            strcpy(error, "STACK_OVERFLOW");
            break;
        case GL_STACK_UNDERFLOW:
            strcpy(error, "STACK_UNDERFLOW");
            break;
        case GL_OUT_OF_MEMORY:
            strcpy(error, "OUT_OF_MEMORY");
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            strcpy(error, "INVALID_FRAMEBUFFER_OPERATION");
            break;
        }
        printf("%s | %s (%d)\n", error, file, line);
        // std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}

#define glCheckError() glCheckError_(__FILE__, __LINE__)

#endif
