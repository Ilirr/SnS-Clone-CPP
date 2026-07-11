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
		auto sprite = scene.getSprite(entity);
		if (transform && sprite && sprite->spriteID >= 0)
		{
			glm::vec2 interpPos = transform->prevPosition + (transform->position - transform->prevPosition) * static_cast<float>(alpha);

			SubTexture2D sub = atlas.getSpriteById(sprite->spriteID);
			renderer.drawQuad(interpPos, transform->size, sub, sprite->colorTint);
			
		}
	}
}