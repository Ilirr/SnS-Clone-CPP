#include "WeaponRegistry.h"
#include "Scene.h"
#include <utility>

std::unordered_map<WeaponType, WeaponDefinition, WeaponTypeHash> WeaponRegistry::s_defs;
WeaponDefinition WeaponRegistry::s_default;

void WeaponRegistry::init(const Atlas& atlas)
{
	s_defs.clear();

	// Sword - use the hero sprite for now (will replace with actual sword sprite later)
	WeaponDefinition sword;
	sword.weaponID = 1;
	sword.spriteID = atlas.getSpriteId("hero");
	sword.damage = 12;
	sword.attackDuration = 0.5f;
	s_defs[WeaponType::Sword] = sword;

	// Spear
	WeaponDefinition spear;
	spear.weaponID = 2;
	spear.spriteID = atlas.getSpriteId("spear");
	spear.damage = 10;
	spear.attackDuration = 0.6f;
	s_defs[WeaponType::Spear] = spear;

	// You can add more weapons here. If atlas doesn't contain the sprite name,
	// spriteID will be -1 which is handled by rendering code.
}

void WeaponRegistry::initForEntity(Scene& scene, EntityID owner, WeaponType type)
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
	weaponComp.state.attackDuration = weaponComp.definition.attackDuration;

	SpriteComponent weaponSprite;
	weaponSprite.spriteID = weaponComp.definition.spriteID;

	ColliderComponent weaponCollider;
	weaponCollider.size = weaponTransform.size;
	weaponCollider.owner = owner;

	// If the owner already has a sprite, match its flip state
	auto ownerSprite = scene.getSprite(owner);
	if (ownerSprite) weaponSprite.flipX = ownerSprite->flipX;

	scene.getEntityManager().addComponent(weaponEnt, weaponTransform);
	scene.getEntityManager().addComponent(weaponEnt, weaponSprite);
	scene.getEntityManager().addComponent(weaponEnt, weaponCollider);
	scene.getEntityManager().addComponent(weaponEnt, TagComponent{ "Weapon" });
	scene.getEntityManager().addComponent(owner, weaponComp);
}

const WeaponDefinition& WeaponRegistry::get(WeaponType type)
{
	auto it = s_defs.find(type);
	if (it != s_defs.end()) return it->second;
	return s_default;
}

bool WeaponRegistry::has(WeaponType type)
{
	return s_defs.find(type) != s_defs.end();
}
