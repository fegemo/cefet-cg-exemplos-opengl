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
        void ativa();

        void defineUniformMatriz(std::string, glm::mat4);
        void defineUniformMatriz(std::string, float*);
        void defineUniformArray(std::string, glm::vec4);
        void defineUniformArray(std::string, glm::vec3);
        void defineUniformArray(std::string, glm::vec2);
        void defineUniformArray(std::string, float*, unsigned char);
        void defineUniformBool(std::string, bool);

    private:
        GLuint vertexShaderId;
        GLuint fragmentShaderId;
        GLuint programId;

        std::map<std::string, GLuint> localizacoesUniforms;
        std::string carregaDoArquivo(std::string);
        GLuint compila(GLuint, std::string);
        GLuint liga();

        GLuint pegaLocalizacaoUniform(std::string);
};

#endif // _SHADER_H_
