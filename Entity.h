#pragma once
#include <cstdint>
#include "Components.h"
#include <optional>

using EntityID = struct EntityID
{
	int index; uint32_t generation;

};
namespace std
{
	template<>
	struct hash<EntityID>
	{
		std::size_t operator()(const EntityID& id) const noexcept
		{
			return std::hash<int>()(id.index) ^ (std::hash<uint32_t>()(id.generation) << 1);
		}
	};
}