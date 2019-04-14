#ifndef _LUZ_H_
#define _LUZ_H_

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glm/glm.hpp"

struct Luz {
    glm::vec3 posicao;
    glm::vec3 cor;
};

#endif // _LUZ_H_
