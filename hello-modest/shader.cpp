#include "shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "glm/gtc/type_ptr.hpp" // value_ptr

shader::shader(std::string vertexFilePath, std::string fragmentFilePath)
{
  this->uniformLocations = std::map<std::string, GLuint>();

  // 1. compila o vertex shader
  std::string vertexShaderSource = loadFromFile("shaders/vertexShader.glsl");
  this->vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(this->vertexShaderId, 1,
                 (const char**) vertexShaderSource.c_str(), NULL);
  glCompileShader(this->vertexShaderId);

  // 2. compila o fragment shader
  std::string fragmentShaderSource = loadFromFile("shaders/fragmentShader.glsl");
  this->fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(this->fragmentShaderId, 1,
                 (const char**) fragmentShaderSource.c_str(), NULL);
  glCompileShader(this->fragmentShaderId);


  // 3. linka os dois em um programa
  this->programId = glCreateProgram();
  glAttachShader(this->programId, this->vertexShaderId);
  glAttachShader(this->programId, this->fragmentShaderId);
  glLinkProgram(this->programId);
}

void shader::use()
{
  glUseProgram(this->programId);
}

void shader::setUniformMatrixVariable(std::string variableName, glm::mat4 variableValue)
{
  // verifica se já sabe qual é a localização da variável no programa
  if (this->uniformLocations.find(variableName) == this->uniformLocations.end()) {
    this->uniformLocations[variableName] = glGetUniformLocation(this->programId, variableName.c_str());
  }
  glUniformMatrix4fv(this->uniformLocations[variableName], 1, GL_TRUE,
                     glm::value_ptr(variableValue));
}


std::string shader::loadFromFile(std::string path)
{
  std::stringstream buffer;
  std::ifstream shaderSourceFile(path);

  if (shaderSourceFile.is_open())
  {
    buffer << shaderSourceFile.rdbuf();
    shaderSourceFile.close();
  }

  return buffer.str();
}
