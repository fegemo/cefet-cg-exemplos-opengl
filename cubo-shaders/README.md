# Cubo com VBO e _shaders_

<img src="../docs/cubo-shaders.png" style="float:right; width: 250px">

Um programa que mostra 1 ou 100.000 cubos coloridos em rotação constante em
torno de seu eixo y.

Características:
  - 3D
  - glFrustum/gluPerspective
  - animação de rotação
  - objetos em arrays
  - marcador de FPS
  - mantém razão de aspecto
  - _vertex arrays_
  - _vertex buffer objects_ (vbos)
  - _shaders_

## Objetivo

Mostrar como transformar uma aplicação que usa VBOs em uma que usa VBOs e
shaders. Este exemplo é uma adaptação do [cubos-vbo](../cubos-vbo/) que
tem apenas o modo de armazenamento com VBO e usa _vertex_ e _fragment shaders_
para desenhar.

## Descrição

Este exemplo mostra uma cena com 1 ou 100.000 cubos (aperte tecla <key>m</key>
para alternar) sendo renderizados usando um _shader_ que apenas combina as cores
dos vértices.

Ele usa recursos da linguagem C++ (logo, a extensão `.cpp` dos arquivos) e
a funcionalidade de carregar e usar _shaders_ está no módulo `shader.cpp/.h`,
que possui uma classe `Shader` com essa funcionalidade.

O _vertex shader_ apenas recebe os atributos de cada vértice (coordenadas,
cor e normal), encontra a coordenada transformada por `projection` e
`modelView` e a passa para o _fragment shader_, junto com a cor recebida do
vértice:

```glsl
#version 460 core
#pragma debug(on)
#pragma optimize(off)

layout(location=0) in vec3 coordinate;
layout(location=1) in vec3 normal;
layout(location=2) in vec3 color;

uniform mat4 projection;
uniform mat4 modelView;

out vec4 exportedColor;

void main() {
   gl_Position = projection * modelView * vec4(coordinate, 1.0);
   exportedColor = vec4(color, 1.0);
}
```

O _fragment shader_, por sua vez, recebe apenas a cor interpolada dos vértices
do triângulo de que este fragmento faz parte e define a cor de saída como
essa cor de entrada interpolada:

```glsl
#version 460 core
#pragma debug(on)
#pragma optimize(off)

in vec4 exportedColor;

out vec4 finalColor;

void main() {
   finalColor = exportedColor;
}
```
