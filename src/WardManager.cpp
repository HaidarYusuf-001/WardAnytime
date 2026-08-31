#include "InputHandler.h"
#include "Settings.h"

RE::BSEventNotifyControl InputHandler::ProcessEvent(
    RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>*) {

    if (!a_event) return RE::BSEventNotifyControl::kContinue;

    for (auto event = *a_event; event; event = event->next) {
        auto button = event->AsButtonEvent();
        if (!button || (!button->IsPressed() && !button->IsUp())) continue;

        auto scanCode = static_cast<std::int32_t>(button->GetIDCode());
        bool isDown = button->IsPressed();

        // TEMPORARY DEBUG — hapus setelah masalah ketemu
        if (isDown) {
            std::string msg = "WardAnytime scanCode: " + std::to_string(scanCode);
            RE::DebugNotification(msg.c_str());
        }

        if (scanCode == static_cast<std::int32_t>(Settings::g_mainKey)) {
            _holdingMainKB = isDown;
        }
        if (Settings::g_requireModifier && scanCode == static_cast<std::int32_t>(Settings::g_modifierKey)) {
            _holdingModifierKB = isDown;
        }

        if (Settings::g_gamepadEnable) {
            if (scanCode == Settings::g_gamepadMainButton) {
                _holdingMainGamepad = isDown;
            }
            if (Settings::g_requireModifier && scanCode == Settings::g_gamepadModifierButton) {
                _holdingModifierGamepad = isDown;
            }
        }
    }

    bool combo = ComboPressed();
    if (combo && !_prevComboPressed) {
        _justPressed = true;
    }
    _prevComboPressed = combo;

    return RE::BSEventNotifyControl::kContinue;
}