#include "LevelLoader.h"
#include "PhysicsSystem.h"
#include <fstream>
#include <iostream>

glm::vec2 LevelLoader::loadLevel(const std::string& filePath, PhysicsSystem& physics, float tileSize)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        std::cerr << "Failed to open level file: " << filePath << "\n";
        return glm::vec2(0.0f, 0.0f); // Default spawn if it fails
    }

    std::string line;
    int row = 0;
    glm::vec2 playerSpawn(0.0f, 0.0f);

    // Read the file one line at a time
    while (std::getline(file, line))
    {
        // Loop through every character in that line
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

                // The hitbox perfectly matches the visual size
                block.collider.size = glm::vec2(tileSize, tileSize);
                block.collider.offset = glm::vec2(0.0f, 0.0f);

                physics.addStaticBody(block);
            }
            else if (tile == 'P')
            {
                // We found the player! Save this location to return later.
                playerSpawn = glm::vec2(worldX, worldY);
            }
        }
        row++; // Move down to the next Y coordinate
    }

    file.close();
    return playerSpawn;
}