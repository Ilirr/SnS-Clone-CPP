#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>
#include "Camera2D.h"

class Shader;
class Texture;
class SubTexture2D;
class Renderer2D
{
public:
	Renderer2D();
	~Renderer2D();
	// Attach an external camera; if not set the internal default camera is used
	void setCamera(const Camera2D* camera) { m_Camera = camera; }
	// Access the internal default camera (useful if no external camera is provided)
	Camera2D& getCamera() { return m_DefaultCamera; }
	void onResize(int width, int height);
	void begin(double alpha);
	void end();
	void drawQuad(const glm::vec2& position, const glm::vec2& size, const Texture& tex, const glm::vec4& color);
	void drawQuad(const glm::vec2& position, const glm::vec2& size, const SubTexture2D& subTex, const glm::vec4& color);
	void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
	void flush();
private:
	void init();
	struct QuadVertex
	{
		glm::vec2 position;
		glm::vec4 color;
		glm::vec2 texCoord;
	};
	std::unique_ptr<Shader> shader;
	std::unique_ptr<Texture> texture;
	GLuint m_VAO = 0;
	GLuint m_VBO = 0;
	GLuint m_EBO = 0;
	GLuint m_WhiteTexture = 0;

	static const size_t MaxQuads = 10000;
	static const size_t MaxVertices = MaxQuads * 4;
	static const size_t MaxIndices = MaxQuads * 6;
	uint32_t m_IndexCount = 0;
	QuadVertex* m_QuadBufferBase = nullptr;
	QuadVertex* m_QuadBufferPtr = nullptr;
	const Texture* m_CurrentTexture = nullptr;

	// Camera support: either an external camera pointer or a default internal one
	const Camera2D* m_Camera = nullptr;
	Camera2D m_DefaultCamera;


};