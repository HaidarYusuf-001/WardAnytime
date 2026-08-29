#pragma once

namespace Settings {
    inline std::uint32_t g_mainKey = 0x10;     // DIK_LSHIFT/ 
    inline std::uint32_t g_modifierKey = 0;   
    inline bool g_requireModifier = false;

    void Load();  
}