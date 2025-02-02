#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texPos;

out vec2 tex_coord;

uniform vec3 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position =  projection * view * vec4(aPos, 1.0);
    tex_coord = texPos;
}