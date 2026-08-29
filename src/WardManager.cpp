#include "WardManager.h"
#include "InputHandler.h"

namespace {
    RE::SpellItem* LookupWard(RE::FormID a_localFormID) {
        return RE::TESDataHandler::GetSingleton()->LookupForm<RE::SpellItem>(a_localFormID, "Skyrim.esm");
    }

    constexpr float kLesserWardCostPerSec    = 16.0f;
    constexpr float kSteadfastWardCostPerSec = 27.0f;
    constexpr float kGreaterWardCostPerSec   = 47.0f;
}

RE::SpellItem* WardManager::GetBestOwnedWard(RE::Actor* a_actor) const {
    static auto* greater   = LookupWard(0x000211f0);
    static auto* steadfast = LookupWard(0x000211f1);
    static auto* lesser    = LookupWard(0x00013018);

    for (auto* ward : { greater, steadfast, lesser }) {
        if (ward && a_actor->HasSpell(ward)) {
            return ward;
        }
    }
    return nullptr;
}

float WardManager::GetCostPerSecond(RE::SpellItem* a_ward) const {
    static auto* greater   = LookupWard(0x000211f0);
    static auto* steadfast = LookupWard(0x000211f1);

    if (a_ward == greater)   return kGreaterWardCostPerSec;
    if (a_ward == steadfast) return kSteadfastWardCostPerSec;
    return kLesserWardCostPerSec;
}

bool WardManager::HasEnoughMagicka(RE::Actor* a_actor, RE::SpellItem* a_ward, float a_delta) const {
    float cost = GetCostPerSecond(a_ward) * a_delta;
    auto current = a_actor->AsActorValueOwner()->GetActorValue(RE::ActorValue::kMagicka);
    return current >= cost;
}

void WardManager::Update(float a_delta) {
    auto* player = RE::PlayerCharacter::GetSingleton();
    if (!player) return;

    bool holding = InputHandler::GetSingleton()->IsHoldingWardCombo();
    if (!holding) {
        _wasHolding = false;
        return;
    }

    auto* ward = GetBestOwnedWard(player);
    if (!ward) return;  

    if (!HasEnoughMagicka(player, ward, a_delta)) return;

    float cost = GetCostPerSecond(ward) * a_delta;
    player->AsActorValueOwner()->RestoreActorValue(
        RE::ACTOR_VALUE_MODIFIER::kDamage, RE::ActorValue::kMagicka, -cost);

    auto* caster = player->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
    if (caster) {
        caster->CastSpellImmediate(ward, false, player, 1.0f, false, 0.0f, player);
    }

    _wasHolding = true;
}