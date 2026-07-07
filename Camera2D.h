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

	// This is the magic function that tells OpenGL how to shift the world
	// Implemented inline so Camera.cpp can be removed.
	glm::mat4 getViewMatrix() const
	{
		// Translate the world by the negative camera position so that
		// moving the camera to the right appears to move the world to the left.
		return glm::translate(glm::mat4(1.0f), glm::vec3(-m_position, 0.0f));
	}

	// Interpolated view matrix for smooth rendering between fixed updates
	glm::mat4 getViewMatrixInterpolated(double alpha) const
	{
		glm::vec2 interp = m_prevPosition + (m_position - m_prevPosition) * static_cast<float>(alpha);
		return glm::translate(glm::mat4(1.0f), glm::vec3(-interp, 0.0f));
	}
    

private: 
	glm::vec2 m_position;
	glm::vec2 m_prevPosition;
};