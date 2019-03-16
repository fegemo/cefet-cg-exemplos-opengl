#version 460 core
#pragma debug(on)
#pragma optimize(off)

layout(location=0) in vec3 coordinate;
layout(location=1) in vec3 normal;
layout(location=2) in vec3 color;

uniform mat4 projection;
uniform mat4 modelView;

out vec4 exportedColor;

void main()
{
   gl_Position = projection * modelView * vec4(coordinate, 1.0);
   exportedColor = vec4(color, 1.0);
}
