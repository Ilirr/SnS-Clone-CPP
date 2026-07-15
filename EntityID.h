#pragma once

#include <cstdint>
#include <cstddef>
#include <functional>

struct EntityID
{
	int index = -1;
	uint32_t generation = 0;

	bool isValid() const { return index >= 0; }
	bool operator==(EntityID o) const { return index == o.index && generation == o.generation; }
	bool operator!=(EntityID o) const { return !(*this == o); }
};

struct EntityIDHash
{
	std::size_t operator()(EntityID id) const noexcept
	{
		const std::size_t indexHash = std::hash<int>{}(id.index);
		const std::size_t generationHash = std::hash<uint32_t>{}(id.generation);
		return indexHash ^ (generationHash + 0x9e3779b9u + (indexHash << 6) + (indexHash >> 2));
	}
};
