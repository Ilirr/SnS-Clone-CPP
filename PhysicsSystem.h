#pragma once
#include "CollisionWorld.h"

class Scene;
class Renderer2D;
struct EntityID;

class PhysicsSystem
{
public:
    void onAttach(Scene* scene, CollisionWorld* world);
    void onDetach();

    void updateAll(double dt);
    void renderDebugOverlay(Renderer2D& renderer) const;

    AABB getAABB(EntityID id) const;

private:
    void moveAndResolve(EntityID id, float dt);
    void syncDynamics();

    Scene* m_scene = nullptr;
    CollisionWorld* m_world = nullptr;   // NOT owned — the game / level owns it
};