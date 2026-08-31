#include "Settings.h"
#include <SimpleIni.h>

namespace Settings {
    void Load() {
        CSimpleIniA ini;
        ini.SetUnicode();
        ini.LoadFile(R"(Data\SKSE\Plugins\WardAnytime.ini)");

        g_mainKey = static_cast<std::uint32_t>(ini.GetLongValue("Keybind", "MainKey", 0x10));
        g_modifierKey = static_cast<std::uint32_t>(ini.GetLongValue("Keybind", "ModifierKey", 0));
        g_requireModifier = ini.GetBoolValue("Keybind", "RequireModifier", false);
        g_toggleMode = ini.GetBoolValue("Keybind", "ToggleMode", false);

        g_gamepadEnable = ini.GetBoolValue("Keybind.Gamepad", "Enable", false);
        g_gamepadMainButton = static_cast<std::int32_t>(ini.GetLongValue("Keybind.Gamepad", "MainButton", -1));
        g_gamepadModifierButton = static_cast<std::int32_t>(ini.GetLongValue("Keybind.Gamepad", "ModifierButton", -1));
        g_gamepadLogUnmapped = ini.GetBoolValue("Keybind.Gamepad", "LogUnmappedInput", false);

        g_requireMagicEquipped  = ini.GetBoolValue("Restrictions", "RequireMagicEquipped", false);
        g_requireMeleeEquipped  = ini.GetBoolValue("Restrictions", "RequireMeleeEquipped", false);
        g_requireRangedEquipped = ini.GetBoolValue("Restrictions", "RequireRangedEquipped", false);
        g_requireUnarmed        = ini.GetBoolValue("Restrictions", "RequireUnarmed", false);
        g_requireShielded       = ini.GetBoolValue("Restrictions", "RequireShielded", false);
        g_requireSheathed       = ini.GetBoolValue("Restrictions", "RequireSheathed", false);

        g_mysticismPlugin = ini.GetValue("MysticismWards", "PluginFile", "MysticismMagic.esp");
        g_grandWardFormID = static_cast<std::uint32_t>(ini.GetLongValue("MysticismWards", "GrandWardFormID", 0x00FFF7));
        g_spelldrinkerFormID = static_cast<std::uint32_t>(ini.GetLongValue("MysticismWards", "SpelldrinkerFormID", 0x3DB99C));

        g_autoCastOnBlock = ini.GetBoolValue("AutoCast", "OnBlock", false);

        logger::info("Settings loaded: ToggleMode={} GamepadEnable={} AutoCastOnBlock={} "
            "Restrictions[Magic={} Melee={} Ranged={} Unarmed={} Shielded={} Sheathed={}]",
            g_toggleMode, g_gamepadEnable, g_autoCastOnBlock,
            g_requireMagicEquipped, g_requireMeleeEquipped, g_requireRangedEquipped,
            g_requireUnarmed, g_requireShielded, g_requireSheathed);
    }
}