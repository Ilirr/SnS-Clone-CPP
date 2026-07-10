#pragma once
#include "Components.h"

struct Player {
	TransformComponent m_transformComponent;
	ColliderComponent m_colliderComponent;
	RigidbodyComponent m_rigidbodyComponent;
};
