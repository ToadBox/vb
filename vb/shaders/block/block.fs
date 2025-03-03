#version 330 core

out vec4 FragColor;
in vec2 tex_coord;

uniform sampler2D tex;

void main()
{
    vec4 t = texture(tex, tex_coord);
    // FragColor = vec4(0.2 * t.x, 0.6 * t.y, 0.2 * t.z, t.w); // how to do the overlay coloring
    FragColor = t;
}