#pragma once
#include <vector>
#include <unordered_map>

struct Animation {
    std::vector<int> frameIDs;             
    int frameRate = 12;                     
    bool loop = true;
    std::vector<int> events;
};

enum class AnimState : uint8_t {
    Idle, Run, Jump, Fall,
    Attack, Hurt, Roll, Death
};