#pragma once

#include "../../includes.hxx"

namespace gensokyo::cheats {
	void createMove(CUserCmd* cmd) noexcept;
	void applyMouse(CInput* _this, CUserCmd* cmd, float& mouseX) noexcept;
}