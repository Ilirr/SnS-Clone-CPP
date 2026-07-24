#include "WeaponRegistry.h"
#include "Scene.h"
#include <utility>

void WeaponRegistry::init(const Atlas& atlas)
{
	m_definitions.clear();

	WeaponDefinition sword;
	sword.weaponID = 1;
	sword.damage = 12;
	sword.attackDuration = 0.5f;
	m_definitions[WeaponType::Sword] = sword;

	WeaponDefinition spear;
	spear.weaponID = 2;
	spear.damage = 10;
	spear.attackDuration = 0.6f;
	m_definitions[WeaponType::Spear] = spear;

}

void WeaponRegistry::initForEntity(Scene& scene, EntityID owner, WeaponType type) const
{
	// Guard: owner must have a transform
	TransformComponent* ownerTransform = scene.getTransform(owner);
	if (!ownerTransform) return;

	EntityID weaponEnt = scene.createEntity();

	TransformComponent weaponTransform;
	weaponTransform.size = ownerTransform->size * 0.5f;
	weaponTransform.position.x = ownerTransform->position.x + ownerTransform->size.x - (weaponTransform.size.x * 0.5f);
	weaponTransform.position.y = ownerTransform->position.y + (ownerTransform->size.y * 0.5f) - (weaponTransform.size.y * 0.5f);
	weaponTransform.prevPosition = weaponTransform.position;

	WeaponComponent weaponComp;
	weaponComp.type = type;
	weaponComp.definition = get(type);
	weaponComp.weaponEntity = weaponEnt;
	weaponComp.attackDuration = weaponComp.definition.attackDuration;


	ColliderComponent weaponCollider;
	weaponCollider.size = { 32.0f, 16.0f };
	weaponCollider.offset = {
		weaponTransform.size.x * 0.5f,
		weaponTransform.size.y * 0.5f - weaponCollider.size.y * 0.5f
	};
	weaponCollider.isSolid = false;
	weaponCollider.owner = owner;

	auto ownerSprite = scene.getSprite(owner);

	scene.getEntityManager().addComponent(weaponEnt, weaponTransform);
	scene.getEntityManager().addComponent(weaponEnt, weaponCollider);
	scene.getEntityManager().addComponent(weaponEnt, TagComponent{ "Weapon" });
	scene.getEntityManager().addComponent(owner, weaponComp);
}

const WeaponDefinition& WeaponRegistry::get(WeaponType type) const
{
	auto it = m_definitions.find(type);
	if (it != m_definitions.end()) return it->second;
	return m_default;
}

bool WeaponRegistry::has(WeaponType type) const
{
	return m_definitions.find(type) != m_definitions.end();
}
