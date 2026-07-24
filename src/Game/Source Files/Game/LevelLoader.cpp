#include "LevelLoader.h"
#include "Scene.h"
#include "Atlas.h"
#include "WeaponRegistry.h"
#include "CollisionWorld.h"
#include <fstream>
#include <iostream>
#include <JSON/json.hpp>

namespace
{
    using Json = nlohmann::json;

    bool readPosition(const Json& entity, glm::vec2& position)
    {
        if (!entity.contains("px") || !entity["px"].is_array() || entity["px"].size() < 2)
        {
            return false;
        }

        try
        {
            position = {
                entity["px"][0].get<float>(),
                entity["px"][1].get<float>()
            };
        }
        catch (const Json::exception&)
        {
            return false;
        }

        return true;
    }

    const Json* findLayer(const Json& level, const std::string& identifier)
    {
        if (!level.contains("layerInstances") || !level["layerInstances"].is_array())
        {
            return nullptr;
        }

        for (const Json& layer : level["layerInstances"])
        {
            if (layer.value("__identifier", "") == identifier)
            {
                return &layer;
            }
        }

        return nullptr;
    }

    const Json* findEntity(const Json& level, const std::string& identifier)
    {
        const Json* layer = findLayer(level, "Entities");
        if (!layer || !layer->contains("entityInstances") || !(*layer)["entityInstances"].is_array())
        {
            return nullptr;
        }

        for (const Json& entity : (*layer)["entityInstances"])
        {
            if (entity.value("__identifier", "") == identifier)
            {
                return &entity;
            }
        }

        return nullptr;
    }

    EntityID addVisualEntity(const Json& entity, Scene& scene, Atlas& atlas, const std::string& tag)
    {
        glm::vec2 anchor;
        if (!readPosition(entity, anchor))
        {
            return {};
        }

        const EntityID entityId = scene.createEntity();

        TransformComponent transform;
        transform.size = { entity.value("width", 20.0f), entity.value("height", 38.0f) };
        transform.position = anchor - glm::vec2(transform.size.x * 0.5f, transform.size.y);
        transform.prevPosition = transform.position;

        SpriteComponent sprite;
        sprite.spriteID = atlas.getSpriteId("hero");

        scene.getEntityManager().addComponent(entityId, transform);
        scene.getEntityManager().addComponent(entityId, sprite);
        scene.getEntityManager().addComponent(entityId, TagComponent{ tag });
        return entityId;
    }

    void buildCollisionWorld(const Json& level, CollisionWorld& world, float tileSize)
    {
        // A fresh level means a fresh world: clear stale tiles AND stale dynamics.
        world.clear();

        const Json* layer = findLayer(level, "Collisions");
        if (!layer || !layer->contains("intGridCsv") || !(*layer)["intGridCsv"].is_array())
        {
            std::cerr << "No valid Collisions IntGrid found in LDtk level.\n";
            return;
        }

        const int width = layer->value("__cWid", 0);
        const int height = layer->value("__cHei", 0);
        const Json& cells = (*layer)["intGridCsv"];
        if (width <= 0 || height <= 0 ||
            cells.size() != static_cast<std::size_t>(width * height))
        {
            std::cerr << "Invalid Collisions IntGrid dimensions in LDtk level.\n";
            return;
        }

        // Allocate the flat grid. Cells default to non-solid, so we only mark
        // the ones whose IntGrid value is > 0.
        world.build(width, height, tileSize);

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                if (cells[static_cast<std::size_t>(y * width + x)].get<int>() == 1)
                {

                    world.setTileType(x, y, TileType::Solid);
                }
                else if (cells[static_cast<std::size_t>(y * width + x)].get<int>() == 2)
                {
                    std::cerr << "Tiletype 2 set!.\n";

                    world.setTileType(x, y, TileType::OneWay);

                }
            }
        }
    }

}

glm::vec2 LevelLoader::loadLevel(const std::string& filePath, Scene& scene, Atlas& atlas, const WeaponRegistry& weaponRegistry, CollisionWorld& world, float tileSize)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Failed to open level file: " << filePath << "\n";
        return {};
    }
    try
    {
        const Json project = Json::parse(file);
        if (!project.contains("levels") || !project["levels"].is_array() || project["levels"].empty())
        {
            std::cerr << "No levels found in LDtk project: " << filePath << "\n";
            return {};
        }

        const Json& level = project["levels"].front();
        const glm::vec2 levelSize{
            level.value("pxWid", 0.0f),
            level.value("pxHei", 0.0f)
        };
        if (levelSize.x <= 0.0f || levelSize.y <= 0.0f)
        {
            std::cerr << "Invalid LDtk level dimensions: " << filePath << "\n";
            return {};
        }

        const EntityID backgroundEntity = scene.createEntity();
        TransformComponent backgroundTransform;
        backgroundTransform.size = levelSize;
        backgroundTransform.prevPosition = backgroundTransform.position;

        SpriteComponent backgroundSprite;
        backgroundSprite.spriteID = atlas.getSpriteId("background");

        scene.getEntityManager().addComponent(backgroundEntity, backgroundTransform);
        scene.getEntityManager().addComponent(backgroundEntity, backgroundSprite);

        buildCollisionWorld(level, world, tileSize);

        const Json* hero = findEntity(level, "Hero");
        if (!hero)
        {
            std::cerr << "No Hero entity found in LDtk level: " << filePath << "\n";
            return {};
        }
        const EntityID playerEntity = addVisualEntity(*hero, scene, atlas, "Player");
        if (!playerEntity.isValid())
        {
            std::cerr << "Could not create Hero entity in LDtk level: " << filePath << "\n";
            return {};
        }
        scene.getEntityManager().addComponent(playerEntity, IntentComponent{});
        scene.getEntityManager().addComponent(playerEntity, RigidbodyComponent{});
        scene.getEntityManager().addComponent(playerEntity, ColliderComponent{ { 20.0f, 38.0f } });
        weaponRegistry.initForEntity(scene, playerEntity, WeaponType::Sword);
        scene.setPlayerEntity(playerEntity);

        const Json* enemy = findEntity(level, "EnemyA");
        if (enemy)
        {
            const EntityID enemyEntity = addVisualEntity(*enemy, scene, atlas, "Player");
            scene.getEntityManager().addComponent(enemyEntity, IntentComponent{});
            scene.getEntityManager().addComponent(enemyEntity, RigidbodyComponent{});
            scene.getEntityManager().addComponent(enemyEntity, ColliderComponent{ { 20.0f, 38.0f } });
            weaponRegistry.initForEntity(scene, enemyEntity, WeaponType::Sword);
        }
        return levelSize;
    }
    catch (const Json::parse_error& error)
    {
        std::cerr << "Failed to parse LDtk level '" << filePath << "': " << error.what() << "\n";
    }
    catch (const Json::type_error& error)
    {
        std::cerr << "Invalid LDtk level data in '" << filePath << "': " << error.what() << "\n";
    }

    return {};
}