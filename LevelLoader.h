#pragma once
#include <string>
#include <glm/glm.hpp>
class PhysicsSystem;

class LevelLoader
{
public:
    // Reads the file, builds the static bodies, and returns the player spawn point
    glm::vec2 loadLevel(const std::string& filepath, PhysicsSystem& physics, float tileSize);
};