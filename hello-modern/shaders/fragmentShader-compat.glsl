#version 330 compatibility

attribute vec4 colorsExport;

void main(void)
{
   gl_FragColor = colorsExport;
}