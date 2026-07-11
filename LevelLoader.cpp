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
                EntityID tileEntity = scene.createEntity();
                TransformComponent transform;
                transform.position = { worldX, worldY };
                transform.prevPosition = transform.position;
                transform.size = { tileSize, tileSize };
                scene.getEntityManager().addComponent(tileEntity, transform);
                scene.getEntityManager().addComponent(tileEntity, ColliderComponent{ { tileSize, tileSize } });
            }
            else if (tile == 'P')
            {
                playerSpawn = glm::vec2(worldX, worldY);
                EntityID playerEntity = scene.createEntity();
                TransformComponent transform;
                transform.position = playerSpawn;
                transform.prevPosition = playerSpawn;
                transform.size = { tileSize, tileSize };
                scene.getEntityManager().addComponent(playerEntity, transform);
                scene.getEntityManager().addComponent(playerEntity, SpriteComponent{ "hero", glm::vec4(1.0f) });
                scene.getEntityManager().addComponent(playerEntity, RigidbodyComponent{});
                scene.getEntityManager().addComponent(playerEntity, ColliderComponent{ { tileSize, tileSize } });
                scene.getEntityManager().addComponent(playerEntity, TagComponent{ "Player" });
                scene.setPlayerEntity(playerEntity);
            }
        }
        row++;
    }

    file.close();
    return;
}


// loadLDtk removed — user will supply their own loader implementation.
