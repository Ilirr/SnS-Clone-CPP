#pragma once
#include "Components.h"
#include "../Graphics/Atlas.h"
#include <unordered_map>

struct WeaponTypeHash
{
	size_t operator()(WeaponType t) const noexcept { return static_cast<size_t>(t); }
};

class WeaponRegistry
{
public:
	void init(const Atlas& atlas);
	void initForEntity(class Scene& scene, EntityID owner, WeaponType type) const;
	const WeaponDefinition& get(WeaponType type) const;
	bool has(WeaponType type) const;

private:
	std::unordered_map<WeaponType, WeaponDefinition, WeaponTypeHash> m_definitions;
	WeaponDefinition m_default;
};
