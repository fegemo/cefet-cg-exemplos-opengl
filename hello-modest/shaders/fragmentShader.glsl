#version 430 core
#pragma debug(on)
#pragma optimize(off)

in vec4 colorsExport;

out vec4 colorsOut;

void main(void)
{
   colorsOut = colorsExport;
}
