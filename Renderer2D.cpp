#include "Renderer2D.h"
#include <iostream>
#include <array>
#include <cstddef>
#include "glm/gtc/matrix_transform.hpp"
#include "Shader.h"
#include "SubTexture2D.h"
#include "Atlas.h"


Renderer2D::Renderer2D()
{
	init();

}
Renderer2D::~Renderer2D()
{

    // free CPU staging buffer
    if (m_QuadBufferBase) {
        delete[] m_QuadBufferBase;
        m_QuadBufferBase = nullptr;
        m_QuadBufferPtr = nullptr;
    }

    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
    if (m_WhiteTexture)
    {
		glDeleteTextures(1, &m_WhiteTexture);
		m_WhiteTexture = 0;
    }
}
void Renderer2D::init()
{
    m_QuadBufferBase = new QuadVertex[MaxVertices];
    m_QuadBufferPtr = m_QuadBufferBase;
    m_IndexCount = 0;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader = std::make_unique<Shader>("assets/shaders/basic.vert", "assets/shaders/basic.frag");

    const float initialW = 800.0f;
    const float initialH = 600.0f;
    glViewport(0, 0, static_cast<GLint>(initialW), static_cast<GLint>(initialH));
    glm::mat4 projection = glm::ortho(0.0f, initialW, initialH, 0.0f, -1.0f, 1.0f);

    shader->use();
    shader->setInt("u_Texture", 0); // ensure shader samples from texture unit 0
    shader->mat4("u_Projection", projection); 

    // Generate the repeating Index pattern on the CPU
    uint32_t* indices = new uint32_t[MaxIndices];
    uint32_t offset = 0;

    for (size_t i = 0; i < MaxIndices; i += 6)
    {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;

        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 3;
        indices[i + 5] = offset + 0;

        offset += 4;
    }
    // create VAO, VBO, and EBO
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    // set up vertex format
    glBindVertexArray(m_VAO);

    // allocate buffer for the full dynamic batch (MaxVertices)
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, MaxVertices * sizeof(QuadVertex), nullptr, GL_DYNAMIC_DRAW);

    // upload index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, MaxIndices * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    delete[] indices;

    // position attribute (location = 0) - vec2 in CPU vertex layout
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), reinterpret_cast<void*>(offsetof(QuadVertex, position)));

    // color attribute (location = 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), reinterpret_cast<void*>(offsetof(QuadVertex, color)));

    // texcoord attribute (location = 2)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), reinterpret_cast<void*>(offsetof(QuadVertex, texCoord)));

    // unbind VAO 
    glBindVertexArray(0);

}
void Renderer2D::begin(double alpha)
{
    shader->use();
    // Pass the current view matrix to the shader. Use external camera if set,
    // otherwise fall back to the internal default camera.
    glm::mat4 view;
    if (m_Camera)
        view = m_Camera->getViewMatrixInterpolated(alpha);
    else
        view = m_DefaultCamera.getViewMatrixInterpolated(alpha);

    shader->mat4("u_View", view);
    glBindVertexArray(m_VAO);
   
    m_CurrentTexture = nullptr;  // On a new frame, clear tracked texture so the first draw will bind its texture
    // Reset CPU staging pointers/counts for a fresh batch
    m_QuadBufferPtr = m_QuadBufferBase;
    m_IndexCount = 0;
}
void Renderer2D::end()
{
    flush();
    glBindVertexArray(0);
}

void Renderer2D::flush()
{
    if (m_IndexCount == 0) return;
    if (!m_QuadBufferBase) return;

    // compute size of vertex data to upload
    size_t vertexCount = static_cast<size_t>(m_QuadBufferPtr - m_QuadBufferBase);
    GLsizeiptr size = static_cast<GLsizeiptr>(vertexCount * sizeof(QuadVertex));

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    // upload only the used portion
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, m_QuadBufferBase);

    // draw the accumulated indices
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_IndexCount), GL_UNSIGNED_INT, nullptr);

    // reset for next batch
    m_IndexCount = 0;
    m_QuadBufferPtr = m_QuadBufferBase;
}
void Renderer2D::onResize(int width, int height)
{
    if (!shader || width <= 0 || height <= 0) return;

    glViewport(0, 0, width, height);
    
    const float virtualW = 800.0f, virtualH = 600.0f;
    float windowAspect = float(width) / float(height);

    float virtualAspect = virtualW / virtualH;
    float viewW = virtualW, viewH = virtualH;
    if (windowAspect > virtualAspect) viewW = virtualH * windowAspect;
    else viewH = virtualW / windowAspect;
    glm::mat4 projection = glm::ortho(0.0f, viewW, viewH, 0.0f, -1.0f, 1.0f);
    shader->use();
    shader->mat4("u_Projection", projection);

}
void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Texture& tex, const glm::vec4& color)
{
    // Ensure there's room for 4 more vertices and 6 more indices; if not, flush
    // the current batch to the GPU and start a fresh batch.
    size_t currentVertexCount = static_cast<size_t>(m_QuadBufferPtr - m_QuadBufferBase);
    if (currentVertexCount + 4 > MaxVertices || m_IndexCount + 6 > MaxIndices)
    {
        flush();
    }

    const Texture* incomingTexture = &tex;
    if (incomingTexture != m_CurrentTexture)
    {
        flush();
        m_CurrentTexture = incomingTexture;
        if (m_CurrentTexture)
            m_CurrentTexture->bind(0);
    }

    // Vertex 0 (Top Left)
    m_QuadBufferPtr->position = { position.x, position.y };
    m_QuadBufferPtr->color = color;
    m_QuadBufferPtr->texCoord = { 0.0f, 0.0f };
    m_QuadBufferPtr++; // Jumps forward exactly sizeof(QuadVertex) bytes

    // Vertex 1 (Top Right)
    m_QuadBufferPtr->position = { position.x + size.x, position.y };
    m_QuadBufferPtr->color = color;
    m_QuadBufferPtr->texCoord = { 1.0f, 0.0f };
    m_QuadBufferPtr++;

    // Vertex 2 (Bottom Right)
    m_QuadBufferPtr->position = { position.x + size.x, position.y + size.y };
    m_QuadBufferPtr->color = color;
    m_QuadBufferPtr->texCoord = { 1.0f, 1.0f };
    m_QuadBufferPtr++;

    // Vertex 3 (Bottom Left)
    m_QuadBufferPtr->position = { position.x, position.y + size.y };
    m_QuadBufferPtr->color = color;
    m_QuadBufferPtr->texCoord = { 0.0f, 1.0f };
    m_QuadBufferPtr++;

    // 1 quad = 2 triangles = 6 indices
    m_IndexCount += 6;
}
void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const SubTexture2D& subTex, const glm::vec4& color, bool flipX)
{
    // Safety valve: ensure space
    size_t currentVertexCount = static_cast<size_t>(m_QuadBufferPtr - m_QuadBufferBase);
    if (currentVertexCount + 4 > MaxVertices || m_IndexCount + 6 > MaxIndices)
    {
        flush();

    }
    const Texture* incomingTexture = &subTex.getTexture();
    if (incomingTexture != m_CurrentTexture)
    {
        flush();

        // Update our tracker to the new texture
        m_CurrentTexture = incomingTexture;

        // Bind the new texture to the GPU
        m_CurrentTexture->bind(0);
    }

    const glm::vec2* uv = subTex.getTexCoords();
    // When flipped horizontally we swap the left and right UVs
    const glm::vec2 uv0 = flipX ? uv[1] : uv[0]; // Top Left
    const glm::vec2 uv1 = flipX ? uv[0] : uv[1]; // Top Right
    const glm::vec2 uv2 = flipX ? uv[3] : uv[2]; // Bottom Right
    const glm::vec2 uv3 = flipX ? uv[2] : uv[3]; // Bottom Left

    // Vertex 0 (Top Left)
    m_QuadBufferPtr->position = { position.x, position.y };
    m_QuadBufferPtr->color = color;
    m_QuadBufferPtr->texCoord = uv0;
    m_QuadBufferPtr++;

    // Vertex 1 (Top Right)
    m_QuadBufferPtr->position = { position.x + size.x, position.y };
    m_QuadBufferPtr->color = color;
    m_QuadBufferPtr->texCoord = uv1;
    m_QuadBufferPtr++;

    // Vertex 2 (Bottom Right)
    m_QuadBufferPtr->position = { position.x + size.x, position.y + size.y };
    m_QuadBufferPtr->color = color;
    m_QuadBufferPtr->texCoord = uv2;
    m_QuadBufferPtr++;

    // Vertex 3 (Bottom Left)
    m_QuadBufferPtr->position = { position.x, position.y + size.y };
    m_QuadBufferPtr->color = color;
    m_QuadBufferPtr->texCoord = uv3;
    m_QuadBufferPtr++;

    m_IndexCount += 6;
}
