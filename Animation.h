#pragma once
#include <vector>
#include <unordered_map>

struct AnimFrameEvent {
    int frameIndex = 0;
    // Use a small enum or string tag — keep it simple
    enum class Type : uint8_t { HitActivate, HitDeactivate, Footstep, Sound } type;
    int payload = 0; // e.g. sound ID
};

struct Animation {
    std::vector<int> frameIDs;              // spriteIDs in playback order
    int frameRate = 12;                     // FPS
    bool loop = true;
    std::vector<AnimFrameEvent> events;     // sorted by frameIndex
};

enum class AnimState : uint8_t {
    Idle, Run, Jump, Fall,
    Attack, Hurt, Roll, Death
};

// Holds the *references* to shared animations + which state is active
struct AnimatorComponent {
    AnimState currentState = AnimState::Idle;
    AnimState requestedState = AnimState::Idle;   // queued by gameplay systems

    // Pointers into the AnimationLibrary — cheap to copy, no frame data duped
    std::unordered_map<AnimState, const Animation*> animations;

    // Playback state
    int   currentFrameIndex = 0;
    float timer = 0.0f;
    bool  isFinished = false;

    // Commit semantics: when true, requestedState is ignored until finished
    bool locked = false;
};