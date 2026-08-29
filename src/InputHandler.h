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

    bool IsHoldingWardCombo() const { return _holdingMain && (!Settings::g_requireModifier || _holdingModifier); }

private:
    InputHandler() = default;
    bool _holdingMain = false;
    bool _holdingModifier = false;
};