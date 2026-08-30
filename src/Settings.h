#pragma once

namespace Settings {
    inline std::uint32_t g_mainKey = 0x10;
    inline std::uint32_t g_modifierKey = 0;
    inline bool g_requireModifier = false;

    // Optional mod ward spells (Mysticism)
    inline std::string g_mysticismPlugin = "MysticismMagic.esp";
    inline std::uint32_t g_grandWardFormID = 0x00FFF7;
    inline std::uint32_t g_spelldrinkerFormID = 0x3DB99C;

    // Cost-per-second overrides (tune without recompiling)
    inline float g_lesserWardCost = 16.0f;
    inline float g_steadfastWardCost = 27.0f;
    inline float g_greaterWardCost = 47.0f;
    inline float g_grandWardCost = 60.0f;      // placeholder, tune to taste
    inline float g_spelldrinkerCost = 75.0f;   // placeholder, tune to taste

    void Load();
}