#include "InputHandler.h"
#include "Settings.h"

namespace {
    void Touch(InputHandler::KeyState& a_state, bool a_isDown) {
        a_state.down = a_isDown;
        if (a_isDown) {
            a_state.lastSeen = std::chrono::steady_clock::now();
        }
    }
}

RE::BSEventNotifyControl InputHandler::ProcessEvent(
    RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>*) {

    if (!a_event) return RE::BSEventNotifyControl::kContinue;

    for (auto event = *a_event; event; event = event->next) {
        auto button = event->AsButtonEvent();
        if (!button || (!button->IsPressed() && !button->IsUp())) continue;

        auto scanCode = static_cast<std::int32_t>(button->GetIDCode());
        bool isDown = button->IsPressed();

        if (scanCode == static_cast<std::int32_t>(Settings::g_mainKey)) {
            Touch(_mainKB, isDown);
        }
        if (Settings::g_requireModifier && scanCode == static_cast<std::int32_t>(Settings::g_modifierKey)) {
            Touch(_modifierKB, isDown);
        }

        if (Settings::g_gamepadEnable) {
            if (scanCode == Settings::g_gamepadMainButton) {
                Touch(_mainGamepad, isDown);
            }
            if (Settings::g_requireModifier && scanCode == Settings::g_gamepadModifierButton) {
                Touch(_modifierGamepad, isDown);
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