#pragma once

#include <cstdint>

struct EntityID
{
	int index = -1;
	uint32_t generation = 0;

	bool isValid() const { return index >= 0; }
	bool operator==(const EntityID& o) const { return index == o.index && generation == o.generation; }
	bool operator!=(const EntityID& o) const { return !(*this == o); }
};
