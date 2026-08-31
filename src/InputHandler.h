#pragma once
#include "Settings.h"

class InputHandler : public RE::BSTEventSink<RE::InputEvent*> {
public:
    static InputHandler* GetSingleton() {
        static InputHandler singleton;
        return &singleton;
    }

    RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* a_event,
                                           RE::BSTEventSource<RE::InputEvent*>*) override;

    bool IsComboHeld() const { return ComboPressed(); }

    // Returns true exactly once per rising edge (key-down transition), then resets.
    bool ConsumeJustPressed() {
        bool result = _justPressed;
        _justPressed = false;
        return result;
    }

private:
    InputHandler() = default;

    bool ComboPressed() const {
        bool mainDown = _holdingMainKB || _holdingMainGamepad;
        bool modifierDown = _holdingModifierKB || _holdingModifierGamepad;
        return mainDown && (!Settings::g_requireModifier || modifierDown);
    }

    bool _holdingMainKB = false;
    bool _holdingModifierKB = false;
    bool _holdingMainGamepad = false;
    bool _holdingModifierGamepad = false;

    bool _justPressed = false;
    bool _prevComboPressed = false;
};