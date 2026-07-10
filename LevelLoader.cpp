#include "LevelLoader.h"
#include "PhysicsSystem.h"
#include "EntityManager.h"
#include <fstream>
#include <iostream>
void LevelLoader::loadLevel(const std::string& filePath, PhysicsSystem& physics, Scene& scene, float tileSize)
{
    std::ifstream file(filePath);
    

    if (!file.is_open())
    {
        std::cerr << "Failed to open level file: " << filePath << "\n";
        return;
    }

    std::string line;
    int row = 0;
    glm::vec2 playerSpawn(0.0f, 0.0f);
    // no return value; create entities directly into EntityManager

    while (std::getline(file, line))
    {
        for (int col = 0; col < line.length(); col++)
        {
            char tile = line[col];

            // Convert grid coordinates (row/col) into world coordinates (pixels)
            float worldX = col * tileSize;
            float worldY = row * tileSize;

            if (tile == '#')
            {

            }
            else if (tile == 'P')
            {
                playerSpawn = glm::vec2(worldX, worldY);
                // create player entity at this location
                EntityID playerEntity = scene.createEntity();

                scene.addComponent(playerEntity, TagComponent{ "Player" });
                scene.addComponent(playerEntity, TransformComponent{ playerSpawn, glm::vec2(tileSize, tileSize) });
                scene.addComponent(playerEntity, RigidbodyComponent{ glm::vec2(0.0f, 0.0f), 1.0f });
                scene.createEntity();
            }
        }
        row++;
    }

    file.close();
    return;
}


// loadLDtk removed — user will supply their own loader implementation.
