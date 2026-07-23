#include "PhysicsSystem.h"
#include "Scene.h"
#include "Components.h"
#include "Renderer2D.h"
#include <cmath>

// ── lifecycle ──────────────────────────────────────────────────────────────

void PhysicsSystem::onAttach(Scene* scene, CollisionWorld* world)
{
    m_scene = scene;
    m_world = world;
}

void PhysicsSystem::onDetach()
{
    m_scene = nullptr;
    m_world = nullptr;
}

AABB PhysicsSystem::getAABB(EntityID id) const
{
    const auto* t = m_scene->getTransform(id);
    const auto* c = m_scene->getCollider(id);
    if (!t || !c) return {};

    const glm::vec2 lo = t->position + c->offset;
    return { lo, lo + c->size };
}

void PhysicsSystem::syncDynamics()
{
    if (!m_scene || !m_world) return;

    m_world->clearDynamics();

    for (EntityID id : m_scene->getActiveEntities())
    {
        if (m_scene->getCollider(id) && m_scene->getRigidbody(id))
            m_world->addDynamic(id, getAABB(id));
    }
}

void PhysicsSystem::moveAndResolve(EntityID id, float dt)
{
    auto* tr = m_scene->getTransform(id);
    auto* co = m_scene->getCollider(id);
    auto* rb = m_scene->getRigidbody(id);
    if (!tr || !co || !rb) return;

    const float tile = m_world->tileSize();

    rb->velocity.y += 981.0f * rb->gravityScale * dt;

    tr->position.x += rb->velocity.x * dt;

    {
        const glm::vec2 lo = tr->position + co->offset;
        const glm::vec2 hi = lo + co->size;

        const int x0 = static_cast<int>(std::floor(lo.x / tile));
        const int x1 = static_cast<int>(std::floor(hi.x / tile));
        const int y0 = static_cast<int>(std::floor(lo.y / tile));
        const int y1 = static_cast<int>(std::floor(hi.y / tile));

        for (int ty = y0; ty <= y1; ++ty)
            for (int tx = x0; tx <= x1; ++tx)
            {
                if (!m_world->isSolid(tx, ty)) continue;

                const AABB tb = m_world->tileAABB(tx, ty);

                if (hi.x <= tb.min.x || lo.x >= tb.max.x) continue;
                if (hi.y <= tb.min.y || lo.y >= tb.max.y) continue;

                if (rb->velocity.x > 0.0f)
                    tr->position.x = tb.min.x - co->offset.x - co->size.x;
                else if (rb->velocity.x < 0.0f)
                    tr->position.x = tb.max.x - co->offset.x;

                rb->velocity.x = 0.0f;
            }
    }

    tr->position.y += rb->velocity.y * dt;

    {
        const glm::vec2 lo = tr->position + co->offset;
        const glm::vec2 hi = lo + co->size;

        const int x0 = static_cast<int>(std::floor(lo.x / tile));
        const int x1 = static_cast<int>(std::floor(hi.x / tile));
        const int y0 = static_cast<int>(std::floor(lo.y / tile));
        const int y1 = static_cast<int>(std::floor(hi.y / tile));

        for (int ty = y0; ty <= y1; ++ty)
            for (int tx = x0; tx <= x1; ++tx)
            {
                if (!m_world->isSolid(tx, ty)) continue;

                const AABB tb = m_world->tileAABB(tx, ty);

                if (hi.x <= tb.min.x || lo.x >= tb.max.x) continue;
                if (hi.y <= tb.min.y || lo.y >= tb.max.y) continue;

                if (rb->velocity.y > 0.0f)          // falling → land
                {
                    tr->position.y = tb.min.y - co->offset.y - co->size.y;
                    rb->isGrounded = true;
                }
                else if (rb->velocity.y < 0.0f)     // jumping → bonk
                {
                    tr->position.y = tb.max.y - co->offset.y;
                }

                rb->velocity.y = 0.0f;
            }
    }
}

void PhysicsSystem::updateAll(double dt)
{
    if (!m_scene || !m_world) return;

    for (EntityID id : m_scene->getActiveEntities())
    {
        auto* rb = m_scene->getRigidbody(id);
        auto* tr = m_scene->getTransform(id);
        auto* co = m_scene->getCollider(id);
        if (!rb || !tr || !co) continue;

        rb->isGrounded = false;
        tr->prevPosition = tr->position;

        moveAndResolve(id, static_cast<float>(dt));
    }
    syncDynamics();
}
void PhysicsSystem::renderDebugOverlay(Renderer2D& renderer) const
{
    if (!m_world) return;

    const glm::vec4 tileColor(0.0f, 1.0f, 0.0f, 0.3f);

    for (int ty = 0; ty < m_world->height(); ++ty)
    {
        for (int tx = 0; tx < m_world->width(); ++tx)
        {
            if (!m_world->isSolid(tx, ty)) continue;
            const AABB b = m_world->tileAABB(tx, ty);
            renderer.drawRectOutline(b.min, b.max - b.min, 1.0f, tileColor);
        }
    }
}