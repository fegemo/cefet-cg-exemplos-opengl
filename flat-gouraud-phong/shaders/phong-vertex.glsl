#version 460 core
#pragma debug(on)
#pragma optimize(off)

layout(location=0) in vec3 coordenada;
layout(location=1) in vec3 normal;
layout(location=2) in vec4 cor;

uniform mat4 projecao;
uniform mat4 modeloVisualizacao;


out vec3 coordenadaNoMundo;
out vec3 normalTransformada;
out vec4 corMisturada;

void main() {
    vec4 coordenadaTransformada = (modeloVisualizacao * vec4(coordenada, 1.0));
    gl_Position = projecao * vec4(coordenadaTransformada.xyz, 1.0);

    coordenadaNoMundo = coordenadaTransformada.xyz / coordenadaTransformada.w;

    // pega a normal do vértice e a transforma (inversa da transposta)
    normalTransformada = normalize(transpose(inverse(mat3(modeloVisualizacao))) * normal);

    // passa a cor para o fragment shader
    corMisturada = cor;
}
