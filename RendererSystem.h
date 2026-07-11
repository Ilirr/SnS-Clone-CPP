#pragma once

class Scene;
class PhysicsSystem;
class Renderer2D;
class Atlas;
class RendererSystem 
{

public:
	RendererSystem();
	void renderScene(const Scene& scene, Renderer2D& renderer, Atlas& atlas, double alpha);

};
