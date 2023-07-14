#pragma once

#pragma comment(lib, "d3d9")
#pragma comment(lib, "d3dx9")

/* std library */
#include <Windows.h>
#include <Psapi.h>
#include <iostream>
#include <cstdint>
#include <format>
#include <ctime>
#include <chrono>
#include <d3d9.h>
#include <d3dx9.h>
#include <intrin.h>
#include <DirectXTex.h>
#include <unordered_map>

/* typedefs */
typedef std::int8_t i8;
typedef std::int16_t i16;
typedef std::int32_t i32;
typedef std::int64_t i64;

typedef std::int_fast8_t if8;
typedef std::int_fast16_t if16;
typedef std::int_fast32_t if32;
typedef std::int_fast64_t if64;

typedef std::int_least8_t il8;
typedef std::int_least16_t il6;
typedef std::int_least32_t il32;
typedef std::int_least64_t il64;

typedef std::intmax_t im;

typedef std::intptr_t iptr;

typedef std::uint8_t u8;
typedef std::uint16_t u16;
typedef std::uint32_t u32;
typedef std::uint64_t u64;

typedef std::uint_fast8_t uf8t;
typedef std::uint_fast16_t uf16;
typedef std::uint_fast32_t uf32;
typedef std::uint_fast64_t uf64;

typedef std::uint_least8_t ul8;
typedef std::uint_least16_t ul16;
typedef std::uint_least32_t ul32;
typedef std::uint_least64_t ul64;

typedef std::uintmax_t um;

typedef std::uintptr_t uptr;

/* external includes */
#include "ext/SafetyHook/safetyhook.hpp"

/* cheat includes */
#include "gensokyo/cheats/options.hxx"
#include "math/vector.hxx"
#include "math/matrix.hxx"
#include "math/math.hxx"
#include "gensokyo/util/fnv.hxx"
#include "gensokyo/util/findpattern.hxx"
#include "csgo/sdk/game/usercmd.hxx"
#include "csgo/sdk/public/inputsystem/InputEnums.hxx"
#include "csgo/sdk/public/inputsystem/ButtonCode.hxx"
#include "csgo/sdk/public/inputsystem/AnalogCode.hxx"
#include "csgo/sdk/sdk.hxx"
#include "csgo/sdk/public/interfaces/interfaces.hxx"
#include "csgo/sdk/public/tier0/annotations.hxx"
#include "csgo/sdk/public/tier0/basetypes.hxx"
#include "csgo/sdk/public/color.hxx"
#include "csgo/sdk/public/tier1/iconvar.hxx"
#include "csgo/sdk/public/tier1/interface.hxx"
#include "csgo/sdk/public/appframework/iappsystem.hxx"
#include "csgo/sdk/public/icvar.hxx"
#include "csgo/sdk/common/input_device.hxx"
#include "csgo/sdk/public/inputsystem/iinputsystem.hxx"
#include "csgo/sdk/public/cdll_int.hxx"
#include "csgo/sdk/engine/cdll_engine_int.hxx"
#include "csgo/sdk/public/const.hxx"
#include "csgo/sdk/game/shared/in_buttons.hxx"
#include "csgo/sdk/public/engine/ivdebugoverlay.hxx"
#include "csgo/interfaces/interfaces.hxx"
#include "gensokyo/util/netvar.hxx"
#include "csgo/sdk/uncatEntity.hxx"
#include "csgo/csgo.hxx"
#include "gensokyo/util/util.hxx"
#include "gensokyo/util/logger.hxx"
#include "gensokyo/gui/renderer.hxx"
#include "gensokyo/gui/miku.hxx"
#include "gensokyo/gui/gui.hxx"
#include "gensokyo/cheats/movement.hxx"
#include "gensokyo/cheats/visuals.hxx"
#include "gensokyo/cheats/cheats.hxx"
#include "gensokyo/hooks/hooks.hxx"
