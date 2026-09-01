#pragma once

namespace Settings {
    inline std::uint32_t g_mainKey = 0x10;
    inline std::uint32_t g_modifierKey = 0;
    inline bool g_requireModifier = false;
    inline bool g_toggleMode = false;

    inline bool g_gamepadEnable = false;
    inline std::int32_t g_gamepadMainButton = -1;
    inline std::int32_t g_gamepadModifierButton = -1;
    inline bool g_gamepadLogUnmapped = false;

    // Stance restrictions — if ALL are false, no restriction (Ward always allowed).
    // If one or more are true, Ward is only allowed while in one of the enabled stances.
    inline bool g_requireMagicEquipped = false;
    inline bool g_requireMeleeEquipped = false;
    inline bool g_requireRangedEquipped = false;
    inline bool g_requireUnarmed = false;
    inline bool g_requireShielded = false;
    inline bool g_requireSheathed = false;

    inline std::string g_mysticismPlugin = "MysticismMagic.esp";
    inline std::uint32_t g_grandWardFormID = 0x00FFF7;
    inline std::uint32_t g_spelldrinkerFormID = 0x3DB99C;

    inline bool g_autoCastOnBlock = false;
inline float g_recastInterval = 0.3f;

    void Load();
}