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
    // Optional owner entity: when set, collisions against the owner can be ignored by the physics system
    EntityID owner = EntityID();
};
struct RigidbodyComponent
{
    glm::vec2 velocity = { 0.0f, 0.0f };
    float gravityScale = 1.0f;
    bool isGrounded = false;

    float moveSpeed = 100.0f; 
    float jumpImpulse = -300.0f;
};
struct TagComponent
{
	std::string tag = "";
};
struct WeaponDefinition
{
    int weaponID = -1;
    int damage = 10;
    float attackDuration = 0.5f;
};
enum class WeaponType
{
    None,
    Sword,
    Spear
};
enum class WeaponState {
    Idle,
    Attacking
};
struct WeaponComponent
{
    WeaponType type = WeaponType::None;
    WeaponState state = WeaponState::Idle;
    WeaponDefinition definition;
    EntityID weaponEntity; 
    float attackDuration = 0.5f;
    float attackTimer = 0.0f;
};
struct IntentComponent
{
	bool jump = false;
	bool attack = false;
	glm::vec2 moveDirection = { 0.0f, 0.0f };
};
struct LivingComponent
{
	int health = 100;
	int maxHealth = 100;
	bool isInvincible = false;
};
