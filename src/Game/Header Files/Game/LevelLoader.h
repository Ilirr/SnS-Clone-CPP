#pragma once
#include <string>
#include <glm/glm.hpp>

class PhysicsSystem;
class Scene;
class Atlas;
class WeaponRegistry;
class CollisionWorld;
class LevelLoader
{
public:
    glm::vec2 loadLevel(const std::string& filepath, Scene& scene, Atlas& atlas, const WeaponRegistry& weaponRegistry, CollisionWorld& world, float tileSize);
};