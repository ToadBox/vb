#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texPos;
layout (location = 2) in vec3 biomeColor;

out vec2 tex_coord;
out vec3 biome_color;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    tex_coord = texPos;
    biome_color = biomeColor;
}