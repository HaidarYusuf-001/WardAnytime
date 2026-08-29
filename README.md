# Ward Anywhere

**Ward Anywhere** is an SKSE plugin for Skyrim Special Edition (Anniversary Edition) that decouples the **Ward** spell (Lesser Ward, Steadfast Ward, Greater Ward) from the vanilla "must be equipped in a hand" requirement.

Hold a configurable hotkey (with an optional modifier key) and your Ward goes up — no matter what's equipped in either hand: a weapon, a shield, a spell, both hands, nothing at all, or even fully sheathed. The plugin automatically selects the strongest Ward spell you currently know.

No animation/behavior (HKX) edits. No Nemesis/Pandora patch required. Pure SKSE + native code.

## Why this exists

Ward is one of the most "basic" spells in Skyrim — in a world saturated with magic, the ability to raise a ward against a hostile spell should feel as universally available as blocking with a weapon. Vanilla locks it behind having a free hand and an equip slot, which makes it awkward or outright unusable for melee builds, dual-wielders, and unarmed characters. This plugin removes that restriction while keeping the underlying Ward mechanic completely vanilla — it just lets you invoke it whenever you actually know a Ward spell.

## Features

- **Hold-to-ward from any stance**: 1H weapon, 1H + shield, dual wield weapons, two-handed, dual wield spells, one spell + weapon, unarmed, or sheathed.
- **Auto-selects your best known Ward**: Greater Ward → Steadfast Ward → Lesser Ward, in that priority, based on what you actually have learned. If you don't know any Ward spell, the hotkey simply does nothing.
- **Fully custom keybind**, including an optional modifier key (e.g. hold `Shift + Q`), configured via a plain `.ini` file — nothing hardcoded.
- **Vanilla-accurate mechanics**: real Ward magic effect, real magicka drain over time, real spell absorption against incoming hostile magic. This is the same Ward effect the game already has — the plugin only changes how it gets triggered.
- **No animation changes**: casting this way does not play the normal hand-casting animation. This is intentional — see [Known Limitations](#known-limitations).
- **Compatible with Ward-focused mods** that alter Ward's visuals, absorption behavior, or blocking interactions (tested against [Perfectly Valid Wards](https://www.nexusmods.com/skyrimspecialedition/mods/153401) by GabonZ), since the plugin invokes the same underlying spell effect rather than reimplementing it.

## How it works (technical summary)

Ward is a *Concentration*-type spell; in vanilla, its active magic effect is continuously re-applied via the hand-casting system, which is why it's normally tied to an equipped hand. This plugin instead re-casts your best known Ward spell every tick using `CastingSource::kInstant`, a casting source not tied to any hand slot, while manually deducting magicka to match the spell's cost. Because the resulting active effect is the exact same vanilla Ward effect, all downstream systems (damage absorption, VFX, compatibility with Ward-altering mods) behave exactly as they would with a normally-cast Ward.

## Requirements

- [Address Library for SKSE Plugins](https://www.nexusmods.com/skyrimspecialedition/mods/32444)
- [SKSE64](https://skse.silverlock.org/) matching your Skyrim AE version
- Skyrim Special Edition, Anniversary Edition (AE), build 1.6.640 through 1.6.1170 confirmed working. See [Compatibility](#compatibility).

## Installation

Install like any other SKSE plugin, using your mod manager of choice (Mod Organizer 2, Vortex, etc.):

1. Download the archive.
2. Install it through your mod manager — it already contains the correct `SKSE/Plugins/` folder structure.
3. Enable the plugin/mod entry.
4. Launch the game through SKSE as usual.

No ESP/ESL, no scripts to install manually, no FOMOD choices.

## How to use

Hold your configured hotkey (default: see `WardAnywhere.ini`) while you have at least one Ward spell learned. As long as you hold it and have enough magicka, Ward stays active regardless of what's in your hands. Release the key to drop it, same as vanilla.

## Configuration

Edit `Data/SKSE/Plugins/WardAnywhere.ini`:

```ini
[Keybind]
; DirectInput scan codes (DIK codes), not virtual key codes.
MainKey = 0x10          ; default: Q
ModifierKey = 0x2A      ; default: Left Shift
RequireModifier = true  ; set to false to use MainKey alone
```

Changes take effect on the next game launch.

## Compatibility

- **Behavior/animation mods** (dual-wield blocking/parrying overhauls, block-rework mods, etc.): fully compatible — this plugin never touches behavior files or animation-related flags.
- **Ward-altering mods**: fully compatible, since Ward Anywhere invokes the same vanilla Ward spell effect rather than replacing it. Tested with **Perfectly Valid Wards** by GabonZ.
- **Skyrim AE version**: developed and tested on **1.6.1170**. Should work unmodified on other post-AE builds (1.6.640–1.6.1170) since it relies only on stable, version-independent SKSE/CommonLibSSE APIs (no hardcoded offsets). **Not yet tested on 1.7.104** — hasn't been verified against it to avoid risking an existing modlist, but there is no known reason it wouldn't work.
- **Safe to install and uninstall mid-save.** The plugin doesn't add any persistent forms, scripts, or save data — it only reacts to input while loaded.

## Building from source

This project targets AE only (1.6.640–1.6.1170), built with:

- Visual Studio 2026, Desktop C++ workload
- CMake 4.4.0+
- Ninja 1.13.2+
- vcpkg (manifest mode) with the [Color-Glass Studios vcpkg registry](https://gitlab.com/colorglass/vcpkg-colorglass) for `commonlibsse-ng-ae`

```powershell
git clone https://github.com/HaidarYusuf-001/WardAnywhere.git
cd WardAnywhere
cmake --preset ae
cmake --build --preset ae
```

Output DLL land in `build/ae/`.

## Credits

- [CommonLibSSE-NG](https://gitlab.com/colorglass/vcpkg-colorglass) / [CharmedBaryon](https://github.com/CharmedBaryon/CommonLibSSE-NG)
- [SKSE Team](https://skse.silverlock.org/)
- [GabonZ](https://www.nexusmods.com/profile/GabonZ) for Perfectly Valid Wards, used during compatibility testing
- [powerofthree](https://www.nexusmods.com/profile/powerofthree) for Strange Runes, used during compatibility testing

## License

MIT (or your preferred license — update this section and add a `LICENSE` file before publishing).
