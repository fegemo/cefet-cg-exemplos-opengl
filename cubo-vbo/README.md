# Cubo: imediatista, vertex array e vbo

<img src="../docs/cubo-vbo.gif" style="float:right; width: 250px">

Um programa que mostra 1 ou 100.000 cubos coloridos em rotação constante em
torno de seu eixo y.

Características:
  - 3D
  - glFrustum/gluPerspective
  - animação de rotação
  - objetos em arrays
  - marcador de FPS
  - mantém razão de aspecto
  - modo imediatista
  - _vertex arrays_
  - _vertex buffer objects_ (vbos)
  - _benchmark_

## Objetivo

Mostrar três diferentes formas para comunicar com a placa de vídeo as
informações sobre os vértices (coordenadas, cores, normais): (1) modo
imediatista, (b) vertex array e (c) vertex buffer object - vbo.

A aplicação possibilita a avaliação de desempenho com a métrica de FPS.

## Descrição

A quantidade de vértices enviados por cubo é sempre a mesma (36), mas em
(1) fazemos 3 chamadas da API para cada vértice (glNormal, glColor e glVertex),
em (2) usamos 3 vetores alocados na RAM para alocar o espaço para normais,
cores e coordenadas e em (3) fazemos assim como (2), mas o espaço alocado fica
na VRAM.

O (a) **modo imediatista** do OpenGL é o que usamos quando temos a definição de
um polígono vértice a vértice, quando usamos `glBegin(...)/glEnd()`. Esse modo
é o mais simples de ser usado, mas seu desempenho não é o melhor porque ele
requer uma chamada da API (_ie_, comando do OpenGL) para cada informação de
cada vértice. É como se a placa de vídeo fosse um dragão faminto e estivéssemos
dando comida pra ela com uma colherzinha de chá - uma hora vamos perder o braço!

Para melhorar essa situação, logo na versão 1.1 o OpenGL introduziu os (b)
**vertex arrays**, que consistem em ter as informações dos vértices alocadas
em regiões contíguas da RAM (_ie_, em arrays). Dessa forma, instruímos o OpenGL
a ler esses arrays (um para coordenadas, um para cor, um para normais) com
as informações dos vértices. A chamada de desenho é apenas `glDrawArrays(...)`.

Outra forma mais recente e com desempenho melhor é atingida pelos (c) **vertex
buffer objects**, introduzida em OpenGL 1.5. Ela é praticamente idêntica aos
vertex arrays, com a diferença que a informação dos vértices fica alocada na
VRAM (memória da placa de vídeo).

Ao executar a aplicação, pode ser que o FPS fique constante ou muito próximo
de 60 para qualquer exemplo. Se isso acontecer, é bem provável que sua placa
de vídeo esteja com _vsync_ (ou _freesync_) ativado, o que fixa a taxa de
atualização das aplicações para serem, no máximo, tão rápidas quanto o monitor.

Se esse for o caso, você pode ou (a) desabilitar essa configuração da placa de
vídeo ou (b) executar o programa com a opção de desabilitar esse recurso:

```bash
$ vblank_mode=0 __GL_SYNC_TO_VBLANK=0 ./cubo-vbo
```

Mais informações sobre desabilitar vsync em placas da Intel e da NVIDIA podem
ser encontradas [nesta página][vsync] do Stack Overflow.

[vsync]: https://stackoverflow.com/questions/17196117/disable-vertical-sync-for-glxgears

### Versões do OpenGL

Para saber quais são as versões e o que cada uma alterou na especificação,
você pode consultar a página [_History of OpenGL_][history] do Khronos Group.

[history]: https://www.khronos.org/opengl/wiki/History_of_OpenGL
