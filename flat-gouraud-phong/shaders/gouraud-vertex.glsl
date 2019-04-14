#version 460 core
#pragma debug(on)
#pragma optimize(off)

layout(location=0) in vec3 coordenada;
layout(location=1) in vec3 normal;
layout(location=2) in vec4 cor;

uniform mat4 projecao;
uniform mat4 modeloVisualizacao;
uniform vec3 posicaoLuz;
uniform vec3 corLuz;
uniform bool blinnPhong;

out vec4 corMisturada;

struct Material {
    float coeficienteAmbiente;
    float coeficienteDifuso;
    float coeficienteEspecular;
    float expoenteEspecularidade;
};

void main() {
    Material material = Material(0.05, 0.8, 1.0, 100);


    vec3 coordenadaNoMundo = (modeloVisualizacao * vec4(coordenada, 1.0)).xyz;
    gl_Position = projecao * vec4(coordenadaNoMundo, 1.0);

    // acha a distância da fonte de luz até o vértice, considerando
    // que a posição dela está dada em relação ao sistema do mundo (sem modelView)
    vec3 pontoAteLuz = posicaoLuz- coordenadaNoMundo;
    float distanciaAteLuz = length(pontoAteLuz);
    pontoAteLuz = normalize(pontoAteLuz);

    // pega a normal do vértice e a transforma (inversa da transposta)
    vec3 normalTransformada = normalize(transpose(inverse(mat3(modeloVisualizacao))) * normal);

    // acha o vetor visualização e o halfway, que é o meio do caminho
    // entre visualização e incidência
    vec3 visualizacao = normalize(-coordenadaNoMundo);
    vec3 meioDoCaminho = normalize(pontoAteLuz + visualizacao);
    vec3 reflexaoLuz = normalize(reflect(-pontoAteLuz, normalTransformada));

    // acha a cor da luz atenuada
    vec3 corDaLuzAtenuada = (1/(1 + distanciaAteLuz * 0.005 + pow(distanciaAteLuz, 2) * 0.0001)) * corLuz;

    // calcula as componentes ambiente, difusa, especular
    vec3 ambiente = cor.rgb * material.coeficienteAmbiente;
    vec3 difusa = max(0, dot(pontoAteLuz, normalTransformada)) * corDaLuzAtenuada * cor.rgb * material.coeficienteDifuso;
    vec3 especular;
    if (blinnPhong == true) {
        especular = pow(max(0, dot(meioDoCaminho, normalTransformada)), material.expoenteEspecularidade) * corDaLuzAtenuada * material.coeficienteEspecular;
    } else {
        especular = pow(max(0, dot(reflexaoLuz, visualizacao)), material.expoenteEspecularidade) * corDaLuzAtenuada * material.coeficienteEspecular;
    }

    vec3 corResultante = ambiente + difusa + especular;
    // correção gamma
    vec3 corResultanteCorrigida = pow(corResultante, vec3(1.0/2.2));

    corMisturada = vec4(corResultanteCorrigida, 1.0);
}
