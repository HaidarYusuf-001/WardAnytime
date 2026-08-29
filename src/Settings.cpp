#include "Settings.h"
#include <SimpleIni.h>

namespace Settings {
    void Load() {
        CSimpleIniA ini;
        ini.SetUnicode();
        ini.LoadFile(R"(Data\SKSE\Plugins\WardAnytime.ini)");

        g_mainKey     = static_cast<std::uint32_t>(
            ini.GetLongValue("Keybind", "MainKey", 0x10));   // default: LShift
        g_modifierKey = static_cast<std::uint32_t>(
            ini.GetLongValue("Keybind", "ModifierKey", 0));
        g_requireModifier = ini.GetBoolValue("Keybind", "RequireModifier", false);

        logger::info("Loaded settings: MainKey={:#x} ModifierKey={:#x} RequireModifier={}",
            g_mainKey, g_modifierKey, g_requireModifier);
    }
}
