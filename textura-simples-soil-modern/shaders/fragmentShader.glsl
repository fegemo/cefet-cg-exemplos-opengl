#version 430 core
#pragma debug(on)
#pragma optimize(off)

in vec4 colorsExport;
in vec2 texCoordinate;
uniform sampler2D colorTexture;

out vec4 colorsOut;

void main(void)
{
   colorsOut = texture(colorTexture, texCoordinate);
}
