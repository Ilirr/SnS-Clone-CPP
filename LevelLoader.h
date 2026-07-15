#pragma once
#include <string>
#include <glm/glm.hpp>

class PhysicsSystem;
class Scene;
class Atlas;
class WeaponRegistry;
class LevelLoader
{
public:
    void loadLevel(const std::string& filepath, Scene& scene, Atlas& atlas, const WeaponRegistry& weaponRegistry, float tileSize);
};