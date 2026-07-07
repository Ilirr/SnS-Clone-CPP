#include "RendererSystem.h"
#include "PhysicsSystem.h"
#include "Renderer2D.h"
#include "Atlas.h"
RendererSystem::RendererSystem()
{


}
void RendererSystem::renderScene(const PhysicsSystem& physicsMgr, Renderer2D& renderer, Atlas& atlas, double alpha)
{
	
	const auto& statics = physicsMgr.getStaticBodies();
	for (const auto& s : statics)
	{
		renderer.drawQuad(s.transform.position, s.transform.size, glm::vec4(0.2f, 0.8f, 0.2f, 1.0f));
	}
	const auto& ents = physicsMgr.getEntities();
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