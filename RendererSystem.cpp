#include "RendererSystem.h"
#include "PhysicsSystem.h"
#include "Scene.h"
#include "Renderer2D.h"
#include "Atlas.h"
RendererSystem::RendererSystem(){}
void RendererSystem::renderScene(const Scene& scene, Renderer2D& renderer, Atlas& atlas, double alpha)
{
	for (const auto& entity : scene.getAllEntities())
	{
		renderer.drawQuad(entity.m_transformComponent.position, entity.m_transformComponent.size, glm::vec4(0.2f, 0.8f, 0.2f, 1.0f));
	}
}