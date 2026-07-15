#include "LevelLoader.h"
#include "Scene.h"
#include "Atlas.h"
#include "WeaponRegistry.h"
#include <fstream>
#include <iostream>

void LevelLoader::loadLevel(const std::string& filePath, Scene& scene, Atlas& atlas, const WeaponRegistry& weaponRegistry, float tileSize)
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
                SpriteComponent sprite;
                transform.position = playerSpawn;
                transform.prevPosition = playerSpawn;
                transform.size = { tileSize, tileSize };
                sprite.spriteID = atlas.getSpriteId("hero");
                
                scene.getEntityManager().addComponent(playerEntity, transform);
                scene.getEntityManager().addComponent(playerEntity, sprite);
                // Add intent component so input/actions can be applied to this entity
                scene.getEntityManager().addComponent(playerEntity, IntentComponent{});
                scene.getEntityManager().addComponent(playerEntity, RigidbodyComponent{});
                scene.getEntityManager().addComponent(playerEntity, ColliderComponent{ { tileSize, tileSize } });
                scene.getEntityManager().addComponent(playerEntity, TagComponent{ "Player" });

                weaponRegistry.initForEntity(scene, playerEntity, WeaponType::Sword);

                scene.setPlayerEntity(playerEntity);
            }
			else if (tile == 'E')
			{
				EntityID enemyEntity = scene.createEntity();
				TransformComponent transform;
				SpriteComponent sprite;
				transform.position = { worldX, worldY };
				transform.prevPosition = transform.position;
				transform.size = { tileSize, tileSize };
				sprite.spriteID = atlas.getSpriteId("hero");
				scene.getEntityManager().addComponent(enemyEntity, transform);
				scene.getEntityManager().addComponent(enemyEntity, sprite);
                // Add intent component for enemies as well (so ActionSystem can safely query intent)
                scene.getEntityManager().addComponent(enemyEntity, IntentComponent{});
				scene.getEntityManager().addComponent(enemyEntity, RigidbodyComponent{});
				scene.getEntityManager().addComponent(enemyEntity, ColliderComponent{ { tileSize, tileSize } });
				scene.getEntityManager().addComponent(enemyEntity, TagComponent{ "Enemy" });
                weaponRegistry.initForEntity(scene, enemyEntity, WeaponType::Sword);
			}
        }
        row++;
    }

    file.close();
    return;
}