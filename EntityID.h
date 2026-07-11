#pragma once

#include <cstdint>
#include <cstddef>

struct EntityID
{
	int index = -1;
	uint32_t generation = 0;

	bool isValid() const { return index >= 0; }
	bool operator==(const EntityID& o) const { return index == o.index && generation == o.generation; }
	bool operator!=(const EntityID& o) const { return !(*this == o); }
};

struct EntityIDHash
{
	std::size_t operator()(const EntityID& id) const noexcept
	{
		return (static_cast<std::size_t>(static_cast<uint32_t>(id.index)) << 32) ^ id.generation;
	}
};
