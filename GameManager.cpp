#include "GameManager.h"
#include <GLFW/glfw3.h>
GameManager::GameManager()
{

}
void GameManager::init()
{
	LevelLoader loader;
	glm::vec2 spawn = loader.loadLevel("assets/levels/level1.txt", m_physicsMgr, 64.0f);

	// Create a dynamic entity (AoS) and register with physics system
	Entity ent;
	ent.transform.position = glm::vec2(100.0f, 100.0f);
	ent.transform.size = glm::vec2(64.0f, 64.0f);
	ent.collider.size = ent.transform.size;
	ent.body.velocity = glm::vec2(50.0f, 0.0f); // px/s to the right
	ent.body.gravityScale = 1.0f;
	m_playerEntity = m_physicsMgr.addEntity(ent);

	// Add a ground static body so the dynamic entity can collide and rest on it
	StaticBody ground;
	ground.transform.position = glm::vec2(0.0f, 400.0f); // y position where ground starts
	ground.transform.size = glm::vec2(800.0f, 50.0f); // wide ground
	ground.collider.size = ground.transform.size;
	ground.collider.offset = glm::vec2(0.0f, 0.0f);
	m_physicsMgr.addStaticBody(ground);
}
void GameManager::handleInput(const InputManager& input)
{
	Entity& p = m_physicsMgr.getEntity(m_playerEntity);

	bool left = input.isDown(GLFW_KEY_A) || input.isDown(GLFW_KEY_LEFT);
	bool right = input.isDown(GLFW_KEY_D) || input.isDown(GLFW_KEY_RIGHT);

	if (left && !right) p.body.velocity.x = -p.body.moveSpeed;
	else if (right && !left) p.body.velocity.x = p.body.moveSpeed;
	else p.body.velocity.x = 0.0f;

	if (input.wasPressed(GLFW_KEY_SPACE) && p.body.isGrounded)
	{
		p.body.velocity.y = p.body.jumpImpulse;
		p.body.isGrounded = false;
	}

}
void GameManager::render(Renderer2D& renderer, Atlas& atlas, double alpha) 
{
	// Draw static bodies first (debug colored quads)
	const auto& statics = m_physicsMgr.getStaticBodies();
	for (const auto& s : statics)
	{
		// draw as green debug quad
		renderer.drawQuad(s.transform.position, s.transform.size, glm::vec4(0.2f, 0.8f, 0.2f, 1.0f));
	}
	const auto& ents = m_physicsMgr.getEntities();
	if (!ents.empty())
	{
		for (const auto& e : ents)
		{
			// Interpolate between previous and current position for smooth rendering
			glm::vec2 interpPos = e.transform.prevPosition + (e.transform.position - e.transform.prevPosition) * static_cast<float>(alpha);
			renderer.drawQuad(interpPos, e.transform.size, atlas.getSprite("hero"), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}
void GameManager::update(double dt)
{

	// Let physics system handle full update for this entity (including collisions) via updateAll
	m_physicsMgr.updateAll(dt);
}
