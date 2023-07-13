#pragma once

#include "../../includes.hxx"

namespace gensokyo::util {
	[[nodiscard]] static uintptr_t findBytes(const uintptr_t& start_address, const uintptr_t& end_address, const char* target_pattern) noexcept;
	[[nodiscard]] static uintptr_t findPattern(const char* module, const char* target_pattern) noexcept;

	constexpr void* vFunc(void* _this, size_t index) noexcept {
		return (*static_cast<void***>(_this))[index];
	}
};
