#version 330 core

in vec4 vColor;
in vec2 vTexCoord;
flat in float vTexIndex;
out vec4 FragColor;

uniform sampler2D u_Textures[16];

void main()
{
    vec4 tex = texture(u_Textures[int(vTexIndex)], vTexCoord);
    FragColor = tex * vColor;
}