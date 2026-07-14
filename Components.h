#pragma once
#include <glm/glm.hpp>
#include <string>
#include "EntityID.h"

struct TransformComponent
{
    glm::vec2 position = { 0.0f, 0.0f };
    glm::vec2 prevPosition = { 0.0f, 0.0f };
    glm::vec2 size = { 64.0f, 64.0f };
    float rotation = 0.0f;
};

struct SpriteComponent
{
    glm::vec4 colorTint = { 1.0f, 1.0f, 1.0f, 1.0f };
    int spriteID = -1;
    bool flipX = false;

};

struct ColliderComponent
{
    glm::vec2 size = { 0.0f, 0.0f };
    glm::vec2 offset = { 0.0f, 0.0f };
    bool isSolid = true;
};
struct RigidbodyComponent
{
    glm::vec2 velocity = { 0.0f, 0.0f };
    float gravityScale = 1.0f;
    bool isGrounded = false;

    float moveSpeed = 150.0f; 
    float jumpImpulse = -400.0f; // negative = up
};
struct TagComponent
{
	std::string tag = "";
};
struct WeaponDefinition
{
    int weaponID = -1;
    int spriteID = -1;
    int damage = 10;
    float attackDuration = 0.5f;
};
enum class WeaponType
{
    None,
    Sword,
    Spear
};
struct WeaponState {
    bool isAttacking = false;
    float attackDuration = 0.5f; 
    float attackTimer = 0.0f;
};
struct WeaponComponent
{
    WeaponType type = WeaponType::None;
    WeaponState state;
    WeaponDefinition definition;
    EntityID weaponEntity; // entity id of the separate weapon visual entity (if any)
};
