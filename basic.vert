// Vertex shader for simple colored quads
// Uses a projection matrix uniform (u_Projection) provided by the renderer
#version 330 core

// input vertex attributes: position, color, texcoord
layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

// pass color to fragment shader
out vec4 vColor;
// projection matrix set by the renderer (orthographic)
uniform mat4 u_Projection;

void main()
{
    // forward vertex color
    vColor = aColor;
    // transform position by the projection matrix to clip space
    gl_Position = u_Projection * vec4(aPosition, 0.0, 1.0);
}