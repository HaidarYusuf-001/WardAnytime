#pragma once
#include "Settings.h"
#include <chrono>

class InputHandler : public RE::BSTEventSink<RE::InputEvent*> {
public:
    struct KeyState {
        bool down = false;
        std::chrono::steady_clock::time_point lastSeen{};
    };

    static InputHandler* GetSingleton() {
        static InputHandler singleton;
        return &singleton;
    }

    RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* a_event,
                                           RE::BSTEventSource<RE::InputEvent*>*) override;

    bool IsComboHeld() const { return ComboPressed(); }

    bool ConsumeJustPressed() {
        bool result = _justPressed;
        _justPressed = false;
        return result;
    }

private:
    InputHandler() = default;

    // "Really held" = flagged down AND refreshed recently. Self-heals if an
    // up-event ever gets missed (e.g. very fast taps), instead of sticking forever.
    static bool IsReallyHeld(const KeyState& s) {
        if (!s.down) return false;
        auto elapsed = std::chrono::steady_clock::now() - s.lastSeen;
        return elapsed < std::chrono::milliseconds(150);
    }

    bool ComboPressed() const {
        bool mainDown = IsReallyHeld(_mainKB) || IsReallyHeld(_mainGamepad);
        bool modifierDown = IsReallyHeld(_modifierKB) || IsReallyHeld(_modifierGamepad);
        return mainDown && (!Settings::g_requireModifier || modifierDown);
    }

    KeyState _mainKB, _modifierKB, _mainGamepad, _modifierGamepad;

    bool _justPressed = false;
    bool _prevComboPressed = false;
};