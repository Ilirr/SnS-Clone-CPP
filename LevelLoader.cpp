#include "LevelLoader.h"
#include "PhysicsSystem.h"
#include <fstream>
#include <iostream>
#include <JSON/json.hpp>
glm::vec2 LevelLoader::loadLevel(const std::string& filePath, PhysicsSystem& physics, float tileSize)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        std::cerr << "Failed to open level file: " << filePath << "\n";
        return glm::vec2(0.0f, 0.0f); 
    }

    std::string line;
    int row = 0;
    glm::vec2 playerSpawn(0.0f, 0.0f);

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
                StaticBody block;
                block.transform.position = glm::vec2(worldX, worldY);
                block.transform.size = glm::vec2(tileSize, tileSize);

                block.collider.size = glm::vec2(tileSize, tileSize);
                block.collider.offset = glm::vec2(0.0f, 0.0f);

                physics.addStaticBody(block);
            }
            else if (tile == 'P')
            {
                playerSpawn = glm::vec2(worldX, worldY);
            }
        }
        row++;
    }

    file.close();
    return playerSpawn;
}


using json = nlohmann::json;

glm::vec2 LevelLoader::loadLDtk(const std::string& filePath, PhysicsSystem& physics)
{
    // 1. Create the default spawn coordinate at the very top
    glm::vec2 playerSpawn(0.0f, 0.0f);

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open LDtk file!\n";
        // FIX #1: Return the default vector instead of a blank 'return;'
        return playerSpawn;
    }

    json ldtkData = json::parse(file);
    auto level = ldtkData["levels"][0];

    for (const auto& layer : level["layerInstances"])
    {
        if (layer["__identifier"] == "Tiles")
        {
            float gridSize = layer["__gridSize"];

            for (const auto& tile : layer["gridTiles"])
            {
                float px = tile["px"][0];
                float py = tile["px"][1];

                StaticBody block;
                block.transform.position = glm::vec2(px, py);
                block.transform.size = glm::vec2(gridSize, gridSize);
                block.collider.size = glm::vec2(gridSize, gridSize);

                // Assuming you map your IDs here!

                physics.addStaticBody(block);
            }
        }
        // (Later on, you can add an 'else if' here to read your LDtk "Entities" layer 
        // to actually update that playerSpawn variable!)
    }

    // FIX #2: Fulfill the promise and return the coordinate at the end of the function!
    return playerSpawn;
}