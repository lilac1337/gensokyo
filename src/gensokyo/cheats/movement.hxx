#pragma once

#include "../../includes.hxx"

namespace gensokyo::cheats::movement {
	void bhop(CUserCmd* cmd) noexcept;
	void opti(CInput* _this, const CUserCmd* cmd, float& mouseX) noexcept;
};