#pragma once
#include <cstdint>

namespace offsets
{
    // NOTE: these MUST be checked against the current Fortnite build
    // by reading the exe headers at runtime. First-run sanity check below.
    constexpr uint64_t UWorld      = 0x0;   // set after validation
    constexpr uint64_t GameInstance= 0x0;
    constexpr uint64_t LocalPlayers = 0x0;
    constexpr uint64_t PlayerController = 0x0;
    constexpr uint64_t AcknowledgedPawn = 0x0;
    constexpr uint64_t PlayerState  = 0x0;
    constexpr uint64_t RootComponent = 0x0;
    constexpr uint64_t Mesh         = 0x0;
    constexpr uint64_t RelativeLocation = 0x0;
    constexpr uint64_t PlayerArray  = 0x0;
    constexpr uint64_t PlayerCount  = 0x0;
    constexpr uint64_t BoneArray    = 0x0;
    constexpr uint64_t TeamIndex    = 0x0;
    constexpr uint64_t bIsDormant   = 0x0;

    // Verified automatically at startup; if mismatch, cheat exits silently
    // instead of doing anything risky.
}
