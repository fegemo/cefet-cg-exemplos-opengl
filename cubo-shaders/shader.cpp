#include "shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "glm/gtc/type_ptr.hpp" // value_ptr

using namespace std;

Shader::Shader(string vertexFilePath, string fragmentFilePath) {
    this->uniformLocations = map<string, GLuint>();

    // 1. compila o vertex shader
    this->vertexShaderId = this->compile(GL_VERTEX_SHADER, vertexFilePath);

    // 2. compila o fragment shader
    this->fragmentShaderId = this->compile(GL_FRAGMENT_SHADER, fragmentFilePath);

    // 3. linka os dois em um programa
    this->programId = this->link();
}

GLuint Shader::compile(GLuint shaderType, string filePath) {
    // carrega o código fonte, cria um shader e o compila
    GLuint shaderId = glCreateShader(shaderType);
    string shaderSource = loadFromFile(filePath);
    const char* shaderSourceC = shaderSource.c_str();
    glShaderSource(shaderId, 1, &shaderSourceC, NULL);
    glCompileShader(shaderId);

    // verifica se o shader foi compilado
    GLint isCompiled = 0;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

        GLchar* infoLog = new GLchar[maxLength + 1];
        glGetShaderInfoLog(shaderId, maxLength, &maxLength, infoLog);

        glDeleteShader(shaderId);

        stringstream error;
        error << "Problema ao compilar shader: " << infoLog;
        throw error.str();
    }

    return shaderId;
}

GLuint Shader::link() {
    GLuint programId = glCreateProgram();
    glAttachShader(programId, this->vertexShaderId);
    glAttachShader(programId, this->fragmentShaderId);
    glLinkProgram(programId);

    GLint isLinked = 0;
    glGetProgramiv(programId, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &maxLength);

        GLchar* infoLog = new GLchar[maxLength + 1];
        glGetShaderInfoLog(programId, maxLength, &maxLength, infoLog);

        glDeleteProgram(programId);
        glDeleteShader(this->vertexShaderId);
        glDeleteShader(this->fragmentShaderId);

        stringstream error;
        error << "Problema ao linkar shader: " << infoLog[0];
        throw error.str();
    }

    glDetachShader(programId, this->vertexShaderId);
    glDetachShader(programId, this->fragmentShaderId);

    return programId;
}

void Shader::use() {
    glUseProgram(this->programId);
}

void Shader::setUniformMatrixVariable(string variableName, glm::mat4 variableValue) {
    this->setUniformMatrixVariable(variableName, glm::value_ptr(variableValue));
}

void Shader::setUniformMatrixVariable(string variableName, float* variableValue) {
    // verifica se já sabe qual é a localização da variável no programa
    map<string, GLuint>::iterator uniformLocationIterator;
    uniformLocationIterator = this->uniformLocations.find(variableName);

    if (uniformLocationIterator == this->uniformLocations.end())
    {
        GLuint uniformLocationId = glGetUniformLocation(this->programId, variableName.c_str());
        this->uniformLocations[variableName] = uniformLocationId;
    }
    glUniformMatrix4fv(this->uniformLocations[variableName], 1, GL_FALSE,
                       variableValue);
}


string Shader::loadFromFile(string path) {
    stringstream buffer;
    ifstream shaderSourceFile(path.c_str());

    if (shaderSourceFile.is_open())
    {
        buffer << shaderSourceFile.rdbuf();
        shaderSourceFile.close();
    }

    return buffer.str();
}
