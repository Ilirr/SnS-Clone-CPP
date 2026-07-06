#version 330 core

in vec4 vColor;
in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D u_Texture;

void main()
{
    vec4 tex = texture(u_Texture, vTexCoord);
    FragColor = tex * vColor;
}