#version 430 core
#pragma debug(on)
#pragma optimize(off)

layout(location=0) in vec4 squareCoords;
layout(location=1) in vec4 squareColors;
layout(location=2) in vec2 squareTexCoordinates;

uniform mat4 projMat;
uniform mat4 modelViewMat;

out vec4 colorsExport;
out vec2 texCoordinate;

void main(void)
{
   gl_Position = projMat * modelViewMat * squareCoords;
   colorsExport = squareColors;
   texCoordinate = squareTexCoordinates;
}
