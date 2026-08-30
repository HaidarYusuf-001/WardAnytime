#include "WardManager.h"
#include "InputHandler.h"
#include "Settings.h"

namespace {
    RE::SpellItem* LookupVanillaWard(RE::FormID a_localFormID) {
        return RE::TESDataHandler::GetSingleton()->LookupForm<RE::SpellItem>(a_localFormID, "Skyrim.esm");
    }

    RE::SpellItem* LookupMysticismWard(RE::FormID a_localFormID) {
        return RE::TESDataHandler::GetSingleton()->LookupForm<RE::SpellItem>(
            a_localFormID, Settings::g_mysticismPlugin);
    }
}

RE::SpellItem* WardManager::GetBestOwnedWard(RE::Actor* a_actor) const {
    static auto* spelldrinker = LookupMysticismWard(Settings::g_spelldrinkerFormID);
    static auto* grandWard    = LookupMysticismWard(Settings::g_grandWardFormID);
    static auto* greater      = LookupVanillaWard(0x000211f0);
    static auto* steadfast    = LookupVanillaWard(0x000211f1);
    static auto* lesser       = LookupVanillaWard(0x00013018);

    for (auto* ward : { spelldrinker, grandWard, greater, steadfast, lesser }) {
        if (ward && a_actor->HasSpell(ward)) {
            return ward;
        }
    }
    return nullptr;
}

bool WardManager::HasEnoughMagicka(RE::Actor* a_actor, RE::SpellItem* a_ward, float a_delta) const {
    // Real, final cost as calculated by the game for this actor — already includes
    // perks, enchantments, and any AV changes from other mods (Mysticism, survival, etc.).
    float costPerSecond = a_ward->CalculateMagickaCost(a_actor);
    float cost = costPerSecond * a_delta;
    auto current = a_actor->AsActorValueOwner()->GetActorValue(RE::ActorValue::kMagicka);
    return current >= cost;
}

void WardManager::Update(float a_delta) {
    auto* player = RE::PlayerCharacter::GetSingleton();
    if (!player) return;

    bool holdingHotkey = InputHandler::GetSingleton()->IsHoldingWardCombo();
    bool holdingBlock = Settings::g_autoCastOnBlock && player->IsBlocking();
    bool holding = holdingHotkey || holdingBlock;

    if (!holding) {
        _wasHolding = false;
        return;
    }

    auto* ward = GetBestOwnedWard(player);
    if (!ward) return;

    if (!HasEnoughMagicka(player, ward, a_delta)) return;

    float cost = ward->CalculateMagickaCost(player) * a_delta;
    player->AsActorValueOwner()->RestoreActorValue(
        RE::ACTOR_VALUE_MODIFIER::kDamage, RE::ActorValue::kMagicka, -cost);

    auto* caster = player->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
    if (caster) {
        caster->CastSpellImmediate(ward, false, player, 1.0f, false, 0.0f, player);
    }

    _wasHolding = true;
}