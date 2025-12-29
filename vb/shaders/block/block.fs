#version 330 core

out vec4 FragColor;

in vec2 tex_coord;
in vec3 biome_color;

uniform sampler2D baseTex;
uniform sampler2D overlayTex;

void main()
{
    vec4 base = texture(baseTex, tex_coord);
    float mask = texture(overlayTex, tex_coord).r;

    vec3 tinted = base.rgb * biome_color;
    vec3 finalColor = mix(base.rgb, tinted, mask);

    FragColor = vec4(finalColor, base.a);
}
