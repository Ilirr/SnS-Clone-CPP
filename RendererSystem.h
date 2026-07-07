#pragma once


class PhysicsSystem;
class Renderer2D;
class Atlas;
class RendererSystem 
{

public:
	RendererSystem();
	void renderScene(const PhysicsSystem& physicsMgr, Renderer2D& renderer, Atlas& atlas, double alpha);

};