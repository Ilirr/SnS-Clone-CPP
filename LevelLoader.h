#pragma once
#include <string>
#include <glm/glm.hpp>

class PhysicsSystem;
class Scene;

class LevelLoader
{
public:
    void loadLevel(const std::string& filepath, PhysicsSystem& physics, Scene& scene, float tileSize);
};