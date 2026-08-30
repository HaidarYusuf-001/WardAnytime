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

        g_mysticismPlugin = ini.GetValue("MysticismWards", "PluginFile", "MysticismMagic.esp");
        g_grandWardFormID = static_cast<std::uint32_t>(
            ini.GetLongValue("MysticismWards", "GrandWardFormID", 0x00FFF7));
        g_spelldrinkerFormID = static_cast<std::uint32_t>(
            ini.GetLongValue("MysticismWards", "SpelldrinkerFormID", 0x3DB99C));

        g_autoCastOnBlock = ini.GetBoolValue("AutoCast", "OnBlock", false);

        logger::info("Loaded settings: MainKey={:#x} ModifierKey={:#x} RequireModifier={} MysticismPlugin={} AutoCastOnBlock={}",
            g_mainKey, g_modifierKey, g_requireModifier, g_mysticismPlugin, g_autoCastOnBlock);
    }
}