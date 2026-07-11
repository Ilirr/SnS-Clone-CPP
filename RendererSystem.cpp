#include "RendererSystem.h"
#include "PhysicsSystem.h"
#include "Scene.h"
#include "Renderer2D.h"
#include "Atlas.h"
RendererSystem::RendererSystem(){}
void RendererSystem::renderScene(const Scene& scene, Renderer2D& renderer, Atlas& atlas, double alpha)
{
	for (const auto& entity : scene.getActiveEntities())
	{
		auto transform = scene.getTransform(entity);
		if (transform) 
		{
			renderer.drawQuad(transform->position, transform->size, atlas.getTexture(), glm::vec4(1.0f));
		}
	}
}