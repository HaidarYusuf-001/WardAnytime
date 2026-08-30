#pragma once

namespace Settings {
    inline std::uint32_t g_mainKey = 0x10;
    inline std::uint32_t g_modifierKey = 0;
    inline bool g_requireModifier = false;

    // Optional mod ward spells (Mysticism)
    inline std::string g_mysticismPlugin = "MysticismMagic.esp";
    inline std::uint32_t g_grandWardFormID = 0x00FFF7;
    inline std::uint32_t g_spelldrinkerFormID = 0x3DB99C;

    // Auto-cast Ward whenever the game/other mods report the player as blocking
    inline bool g_autoCastOnBlock = false;

    void Load();
}