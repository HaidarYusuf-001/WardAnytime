#pragma once

class WardManager {
public:
    static WardManager* GetSingleton() {
        static WardManager singleton;
        return &singleton;
    }

    void Update(float a_delta);

private:
    RE::SpellItem* GetBestOwnedWard(RE::Actor* a_actor) const;
    bool HasEnoughMagicka(RE::Actor* a_actor, RE::SpellItem* a_ward, float a_delta) const;

    bool _wasHolding = false;
};