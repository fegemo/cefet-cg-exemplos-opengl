#version 460 core
#pragma debug(on)
#pragma optimize(off)

in vec4 corMisturada;

out vec4 corFinal;

void main() {
    corFinal = corMisturada;
}
