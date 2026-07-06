#version 330 core

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 vColor;
out vec2 vTexCoord;

uniform mat4 u_Projection;

void main()
{
    vColor = aColor;
    vTexCoord = aTexCoord;
    gl_Position = u_Projection * vec4(aPosition, 0.0, 1.0);
}