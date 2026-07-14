#pragma once
#include "Components.h"
#include "Atlas.h"
#include <unordered_map>

struct WeaponTypeHash
{
	size_t operator()(WeaponType t) const noexcept { return static_cast<size_t>(t); }
};

class WeaponRegistry
{
public:
	static void init(const Atlas& atlas);
	static void initForEntity(class Scene& scene, EntityID owner, WeaponType type);
	static const WeaponDefinition& get(WeaponType type);
	static bool has(WeaponType type);

private:
	static std::unordered_map<WeaponType, WeaponDefinition, WeaponTypeHash> s_defs;
	static WeaponDefinition s_default;
};
