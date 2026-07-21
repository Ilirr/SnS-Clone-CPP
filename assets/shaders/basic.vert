#version 330 core

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aTexIndex;

out vec4 vColor;
out vec2 vTexCoord;
flat out float vTexIndex;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
    vColor = aColor;
    vTexCoord = aTexCoord;
    vTexIndex = aTexIndex;
    gl_Position = u_Projection * u_View * vec4(aPosition, 0.0, 1.0);
}