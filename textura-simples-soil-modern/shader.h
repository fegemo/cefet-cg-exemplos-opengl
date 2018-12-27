#ifndef _SHADER_H_
#define _SHADER_H_

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glm/mat4x4.hpp"
#include <string>
#include <map>

class Shader
{
    public:
        Shader(std::string, std::string);
        void use();

        void setUniformMatrixVariable(std::string, glm::mat4);
        void setUniformMatrixVariable(std::string, float*);
        void setUniformUintVariable(std::string, GLuint);

    private:
        GLuint vertexShaderId;
        GLuint fragmentShaderId;
        GLuint programId;

        std::map<std::string, GLuint> uniformLocations;
        std::string loadFromFile(std::string);
        GLuint compile(GLuint, std::string);
        GLuint link();
};

#endif // _SHADER_H_
