#include "InputHandler.h"
#include "Settings.h"

RE::BSEventNotifyControl InputHandler::ProcessEvent(
    RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>*) {

    if (!a_event) return RE::BSEventNotifyControl::kContinue;

    for (auto event = *a_event; event; event = event->next) {
        auto button = event->AsButtonEvent();
        if (!button || !button->IsPressed() && !button->IsUp()) continue;
        if (button->device.get() != RE::INPUT_DEVICE::kKeyboard) continue;

        auto scanCode = button->GetIDCode();
        bool isDown = button->IsPressed();

        if (scanCode == Settings::g_mainKey) {
            _holdingMain = isDown;
        }
        if (Settings::g_requireModifier && scanCode == Settings::g_modifierKey) {
            _holdingModifier = isDown;
        }
    }
    return RE::BSEventNotifyControl::kContinue;
}