#version 460 core
#pragma debug(on)
#pragma optimize(off)

in vec4 exportedColor;

out vec4 finalColor;

void main()
{
   finalColor = exportedColor;
}
