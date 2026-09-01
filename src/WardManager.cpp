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

    // ---- Hand-content helpers ----
    bool IsHandSpell(RE::Actor* a_actor, bool a_left) {
        auto* eq = a_actor->GetEquippedObject(a_left);
        return eq && eq->Is(RE::FormType::Spell);
    }

    bool IsHandShield(RE::Actor* a_actor, bool a_left) {
        auto* eq = a_actor->GetEquippedObject(a_left);
        return eq && eq->Is(RE::FormType::Armor);
    }

    bool IsHandRangedWeapon(RE::Actor* a_actor, bool a_left) {
        auto* eq = a_actor->GetEquippedObject(a_left);
        auto* weap = eq ? eq->As<RE::TESObjectWEAP>() : nullptr;
        return weap && (weap->IsBow() || weap->IsCrossbow());
    }

    bool IsHandMeleeWeapon(RE::Actor* a_actor, bool a_left) {
        auto* eq = a_actor->GetEquippedObject(a_left);
        auto* weap = eq ? eq->As<RE::TESObjectWEAP>() : nullptr;
        return weap && !weap->IsBow() && !weap->IsCrossbow() && !weap->IsStaff();
    }

    // ---- Stance checks ----
    bool IsMagicStance(RE::Actor* a_actor) {
        return a_actor->AsActorState()->IsWeaponDrawn() && (IsHandSpell(a_actor, true) || IsHandSpell(a_actor, false));
    }
    bool IsMeleeStance(RE::Actor* a_actor) {
        return a_actor->AsActorState()->IsWeaponDrawn() && (IsHandMeleeWeapon(a_actor, true) || IsHandMeleeWeapon(a_actor, false));
    }
    bool IsRangedStance(RE::Actor* a_actor) {
        return a_actor->AsActorState()->IsWeaponDrawn() && (IsHandRangedWeapon(a_actor, true) || IsHandRangedWeapon(a_actor, false));
    }
    bool IsUnarmedStance(RE::Actor* a_actor) {
        return a_actor->AsActorState()->IsWeaponDrawn() && !a_actor->GetEquippedObject(true) && !a_actor->GetEquippedObject(false);
    }
    bool IsShieldStance(RE::Actor* a_actor) {
        return a_actor->AsActorState()->IsWeaponDrawn() && IsHandShield(a_actor, true);
    }
    bool IsSheathedStance(RE::Actor* a_actor) {
        return !a_actor->AsActorState()->IsWeaponDrawn();
    }

    bool StanceAllowed(RE::Actor* a_actor) {
        bool anyRestrictionActive =
            Settings::g_requireMagicEquipped || Settings::g_requireMeleeEquipped ||
            Settings::g_requireRangedEquipped || Settings::g_requireUnarmed ||
            Settings::g_requireShielded || Settings::g_requireSheathed;

        if (!anyRestrictionActive) return true;  // no restriction configured -> always allowed

        if (Settings::g_requireMagicEquipped  && IsMagicStance(a_actor))    return true;
        if (Settings::g_requireMeleeEquipped  && IsMeleeStance(a_actor))    return true;
        if (Settings::g_requireRangedEquipped && IsRangedStance(a_actor))   return true;
        if (Settings::g_requireUnarmed        && IsUnarmedStance(a_actor))  return true;
        if (Settings::g_requireShielded       && IsShieldStance(a_actor))   return true;
        if (Settings::g_requireSheathed       && IsSheathedStance(a_actor)) return true;

        return false;
    }

void DispelActiveWard(RE::Actor* a_actor, RE::SpellItem* a_ward) {
        if (!a_ward) return;
        auto* list = a_actor->AsMagicTarget()->GetActiveEffectList();
        if (!list) return;
        for (auto* effect : *list) {
            if (effect && effect->spell == a_ward) {
                effect->Dispel(true);
            }
        }
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
    float cost = a_ward->CalculateMagickaCost(a_actor) * a_delta;
    auto current = a_actor->AsActorValueOwner()->GetActorValue(RE::ActorValue::kMagicka);
    return current >= cost;
}

void WardManager::Update(float a_delta) {
    auto* player = RE::PlayerCharacter::GetSingleton();
    if (!player) return;

    bool stanceOk = StanceAllowed(player);

    if (Settings::g_toggleMode && _toggleState && !stanceOk) {
        _toggleState = false;
    }

    bool justPressed = InputHandler::GetSingleton()->ConsumeJustPressed();
    if (Settings::g_toggleMode) {
        if (justPressed && stanceOk) {
            _toggleState = !_toggleState;
        }
    }

    bool holdingHotkey = Settings::g_toggleMode
        ? _toggleState
        : InputHandler::GetSingleton()->IsComboHeld();

    bool holdingBlock = Settings::g_autoCastOnBlock && player->IsBlocking();
    bool wantHolding = (holdingHotkey || holdingBlock) && stanceOk;

    RE::SpellItem* ward = wantHolding ? GetBestOwnedWard(player) : nullptr;
    if (wantHolding && ward && !HasEnoughMagicka(player, ward, a_delta)) {
        wantHolding = false;
    }

        if (wantHolding && ward) {
        float cost = ward->CalculateMagickaCost(player) * a_delta;
        player->AsActorValueOwner()->RestoreActorValue(
            RE::ACTOR_VALUE_MODIFIER::kDamage, RE::ActorValue::kMagicka, -cost);

        bool isFirstCast = !_wasHolding || _activeWard != ward;
        bool needsCast = isFirstCast || _timeSinceLastCast >= Settings::g_recastInterval;

        if (needsCast) {
            if (_activeWard && _activeWard != ward) {
                DispelActiveWard(player, _activeWard);
            }
            auto* caster = player->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
            if (caster) {
                // VFX always on - suppressing it kills the persistent shield visual
                // entirely, not just the initial burst.
                caster->CastSpellImmediate(ward, false, player, 1.0f, false, 0.0f, player);
            }
            _activeWard = ward;
            _timeSinceLastCast = 0.0f;
        } else {
            _timeSinceLastCast += a_delta;
        }
        _wasHolding = true;
    } else {
        if (_wasHolding && _activeWard) {
            DispelActiveWard(player, _activeWard);
        }
        _activeWard = nullptr;
        _timeSinceLastCast = 0.0f;
        _wasHolding = false;
    }
}