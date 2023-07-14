#include "../../includes.hxx"

void gensokyo::cheats::createMove(CUserCmd* cmd) noexcept {
	if (!csgo::localPlayer)
		csgo::localPlayer = csgo::interfaces::EntityList->GetClientEntity(csgo::interfaces::Engine->GetLocalPlayer());

	movement::bhop(cmd);

	visuals::alwaysShowCrosshair();
	visuals::nameTags();
	visuals::radar();
}

void gensokyo::cheats::applyMouse(CInput* _this, CUserCmd* cmd, float& mouseX) noexcept {
	movement::opti(_this, cmd, mouseX);
}
