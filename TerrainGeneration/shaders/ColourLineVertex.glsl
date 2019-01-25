#version 410 core

layout (location = 0) in vec3 inVert;
layout (location = 1) in vec4 inColour;
out vec4 colour;

uniform mat4 MVP;

void main()
{
    colour = inColour;
    gl_Position = MVP*vec4(inVert, 1.0);
}

