#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera2D {
public:
	Camera2D() : m_position(0.0f, 0.0f), m_prevPosition(0.0f, 0.0f) {}
	~Camera2D() = default;

	void setPosition(const glm::vec2& position) { m_position = position; }
	void setPrevPosition(const glm::vec2& position) { m_prevPosition = position; }

	const glm::vec2& getPosition() const { return m_position; }
	const glm::vec2& getPrevPosition() const { return m_prevPosition; }

	const float getMaxCameraX() const { return m_maxCameraX; }
	const float getMaxCameraY() const { return m_maxCameraY; }
	const float getViewportWidth() const { return m_viewportWidth; }
	const float getViewportHeight() const { return m_viewportHeight; }

	void setMaxCameraX(const float maxCameraX) { m_maxCameraX = maxCameraX; }
	void setMaxCameraY(const float maxCameraY) { m_maxCameraY = maxCameraY; }

	glm::mat4 getViewMatrixInterpolated(double alpha) const
	{
		glm::vec2 interp = m_prevPosition + (m_position - m_prevPosition) * static_cast<float>(alpha);
		return glm::translate(glm::mat4(1.0f), glm::vec3(-interp, 0.0f));
	}

private:
	glm::vec2 m_position;
	glm::vec2 m_prevPosition;
	const float m_viewportWidth = 320.0f;
	const float m_viewportHeight = 180.0f;
	float m_maxCameraX = 0;
	float m_maxCameraY = 0;
};