#include "shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "glm/gtc/type_ptr.hpp" // value_ptr

using namespace std;

Shader::Shader(string caminhoParaVertexShader, string caminhoParaFragmentShader) {
    this->localizacoesUniforms = map<string, GLuint>();

    // 1. compila o vertex shader
    this->vertexShaderId = this->compila(GL_VERTEX_SHADER, caminhoParaVertexShader);

    // 2. compila o fragment shader
    this->fragmentShaderId = this->compila(GL_FRAGMENT_SHADER, caminhoParaFragmentShader);

    // 3. linka os dois em um programa
    this->programId = this->liga();
}

GLuint Shader::compila(GLuint tipoShader, string caminhoArquivo) {
    // carrega o código fonte, cria um shader e o compila
    GLuint shaderId = glCreateShader(tipoShader);
    string codigoFonteShader = carregaDoArquivo(caminhoArquivo);
    const char* codigoFonteShaderC = codigoFonteShader.c_str();
    glShaderSource(shaderId, 1, &codigoFonteShaderC, NULL);
    glCompileShader(shaderId);

    // verifica se o shader foi compilado
    GLint foiCompilado = 0;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &foiCompilado);
    if(foiCompilado == GL_FALSE) {
        GLint tamanho = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &tamanho);

        GLchar* infoLog = new GLchar[tamanho + 1];
        glGetShaderInfoLog(shaderId, tamanho, &tamanho, infoLog);

        glDeleteShader(shaderId);

        stringstream error;
        error << "Problema ao compilar shader: " << infoLog;
        throw error.str();
    }

    return shaderId;
}

GLuint Shader::liga() {
    GLuint programId = glCreateProgram();
    glAttachShader(programId, this->vertexShaderId);
    glAttachShader(programId, this->fragmentShaderId);
    glLinkProgram(programId);

    GLint foiLigado = 0;
    glGetProgramiv(programId, GL_LINK_STATUS, (int *)&foiLigado);
    if (foiLigado == GL_FALSE) {
        GLint tamanho = 0;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &tamanho);

        GLchar* infoLog = new GLchar[tamanho + 1];
        glGetProgramInfoLog(programId, tamanho, &tamanho, infoLog);

        glDeleteProgram(programId);
        glDeleteShader(this->vertexShaderId);
        glDeleteShader(this->fragmentShaderId);

        stringstream error;
        error << "Problema ao linkar shader: " << infoLog;
        throw error.str();
    }

    glDetachShader(programId, this->vertexShaderId);
    glDetachShader(programId, this->fragmentShaderId);

    return programId;
}

void Shader::ativa() {
    glUseProgram(this->programId);
}

void Shader::defineUniformMatriz(string nome, glm::mat4 valor) {
    this->defineUniformMatriz(nome, glm::value_ptr(valor));
}

void Shader::defineUniformMatriz(string nome, float* valor) {
    GLuint localizacao = this->pegaLocalizacaoUniform(nome);
    glUniformMatrix4fv(localizacao, 1, GL_FALSE, valor);
}


void Shader::defineUniformArray(string nome, glm::vec4 valor) {
    this->defineUniformArray(nome, glm::value_ptr(valor), 4);
}

void Shader::defineUniformArray(string nome, glm::vec3 valor) {
    this->defineUniformArray(nome, glm::value_ptr(valor), 3);
}

void Shader::defineUniformArray(string nome, glm::vec2 valor) {
    this->defineUniformArray(nome, glm::value_ptr(valor), 2);
}

void Shader::defineUniformArray(string nome, float* valor, unsigned char length) {
    GLuint localizacao = this->pegaLocalizacaoUniform(nome);

    if (length == 4) {
        glUniform4fv(localizacao, 1, valor);
    } else if (length == 3) {
        glUniform3fv(localizacao, 1, valor);
    } else if (length == 2) {
        glUniform2fv(localizacao, 1, valor);
    }
}

void Shader::defineUniformBool(std::string nome, bool valor) {
    GLuint localizacao = this->pegaLocalizacaoUniform(nome);

    glUniform1i(localizacao, valor == true ? 1 : 0);
}

GLuint Shader::pegaLocalizacaoUniform(std::string nome) {
    // verifica se já sabe qual é a localização da variável no programa
    GLuint localizacaoId = -1;
    map<string, GLuint>::iterator localizacaoIterator;
    localizacaoIterator = this->localizacoesUniforms.find(nome);

    if (localizacaoIterator == this->localizacoesUniforms.end()) {
        localizacaoId = glGetUniformLocation(this->programId, nome.c_str());
        this->localizacoesUniforms[nome] = localizacaoId;
    } else {
        localizacaoId = this->localizacoesUniforms[nome];
    }

    return localizacaoId;
}


string Shader::carregaDoArquivo(string caminho) {
    stringstream buffer;
    ifstream codigoFonteShaderFile(caminho.c_str());

    if (codigoFonteShaderFile.is_open()) {
        buffer << codigoFonteShaderFile.rdbuf();
        codigoFonteShaderFile.close();
    }

    return buffer.str();
}
