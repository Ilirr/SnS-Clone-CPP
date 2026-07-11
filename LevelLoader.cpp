#include "LevelLoader.h"
#include "Scene.h"
#include "Atlas.h"
#include <fstream>
#include <iostream>

void LevelLoader::loadLevel(const std::string& filePath, Scene& scene, Atlas& atlas, float tileSize)
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

    while (std::getline(file, line))
    {
        for (int col = 0; col < line.length(); col++)
        {
            char tile = line[col];

            float worldX = col * tileSize;
            float worldY = row * tileSize;

            if (tile == '#')
            {
                EntityID tileEntity = scene.createEntity();
                TransformComponent transform;
                SpriteComponent sprite;

                transform.position = { worldX, worldY };
                transform.prevPosition = transform.position;
                transform.size = { tileSize, tileSize };
                sprite.spriteID = atlas.getSpriteId("hero");

                scene.getEntityManager().addComponent(tileEntity, transform);
                scene.getEntityManager().addComponent(tileEntity, ColliderComponent{ { tileSize, tileSize } });
                scene.getEntityManager().addComponent(tileEntity, sprite);

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
                SpriteComponent sprite;
                sprite.spriteID = atlas.getSpriteId("hero");
                scene.getEntityManager().addComponent(playerEntity, sprite);
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