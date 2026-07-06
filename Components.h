#pragma once
#include <glm/glm.hpp>

struct Transform
{
    glm::vec2 position = { 0.0f, 0.0f };
    glm::vec2 size = { 64.0f, 64.0f };
    float rotation = 0.0f;
};

struct Collider
{
    glm::vec2 size = { 0.0f, 0.0f };
    glm::vec2 offset = { 0.0f, 0.0f };
    bool isSolid = true;
};
struct RigidBody
{
    glm::vec2 velocity = { 0.0f, 0.0f };
    float gravityScale = 1.0f;
    bool isGrounded = false;
};

// Simple Entity bundling (Array-of-Structs style)
struct Entity
{
    Transform transform;
    Collider collider;
    RigidBody body;
};

// Static body combining transform and collider for static geometry
struct StaticBody
{
    Transform transform;
    Collider collider;
};


