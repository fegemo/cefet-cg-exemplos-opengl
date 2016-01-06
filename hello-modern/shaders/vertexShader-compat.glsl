#version 330 compatibility

attribute vec4 squareCoords;
attribute vec4 squareColors;

uniform mat4 projMat;
uniform mat4 modelViewMat;

varying vec4 colorsExport;

void main(void)
{
   gl_Position = projMat * modelViewMat * squareCoords;
   colorsExport = squareColors;
}