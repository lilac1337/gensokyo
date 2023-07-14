#include <deque>

#include "../../includes.hxx"

void gensokyo::cheats::movement::bhop(CUserCmd* cmd) noexcept {
	if (!options::movement::bhopEnabled)
		return;

	const i32 flags = csgo::localPlayer->m_fFlags();

	if (!(flags & FL_ONGROUND) && cmd->buttons & IN_JUMP) {
		cmd->buttons &= ~IN_JUMP;
	}
}

void gensokyo::cheats::movement::opti(CInput* _this, const CUserCmd* cmd, float& mouseX) noexcept {
	if (!options::movement::optiEnabled || hooks::keySpeed == 0.f)
		return;

	const i32 buttons = _this->GetButtonBits(0);
	const i32 flags = csgo::localPlayer->m_fFlags();
	
	Vector2<float> velocity = { csgo::localPlayer->m_vecVelocity().x, csgo::localPlayer->m_vecVelocity().y };
	
	const float velLength = velocity.twoDLength();

	static float lastVelLength = velLength;

	const bool jumping = (!(flags & FL_ONGROUND) || (buttons & IN_JUMP));
	const bool aligned = (options::movement::alignedDir) ? ((cmd->sidemove < 0.f && mouseX < 0.f) || (cmd->sidemove > 0.f && mouseX > 0.f)) : true;
	const bool antiWall = (options::movement::antiWall) ? velLength > 260.f && velLength - lastVelLength >= 0 : true;
	
	if (jumping && antiWall && aligned && mouseX && cmd->sidemove) {
		static ConVar* mYaw = csgo::interfaces::CVar->FindVar("m_yaw");
		static ConVar* sensitivity = csgo::interfaces::CVar->FindVar("sensitivity");

		static float degTurned{};

		const float targetDelta = math::rad2deg(((options::movement::useAtan) ? std::atan2f(30.f, velLength) : std::asin(30.f / velLength)));
		const float smoothedDelta = (cmd->tick_count || cmd->command_number) ? targetDelta - degTurned : targetDelta* (hooks::keySpeed / csgo::globals->interval_per_tick);

		degTurned = (cmd->tick_count || cmd->command_number) ? 0.f : degTurned + smoothedDelta;

		const float targetMx = std::copysign(smoothedDelta / mYaw->GetFloat(), (options::movement::useSidemoveDir) ? cmd->sidemove : mouseX);
		const float adjustedMx = mouseX * (1.f - options::movement::optiPercent) + targetMx * options::movement::optiPercent;

		if ((mouseX < 0.f && mouseX > targetMx || mouseX > 0.f && mouseX < targetMx) || !options::movement::onlyOptiUnder)
			mouseX = (options::movement::forceLegalYaw) ? round(adjustedMx / sensitivity->GetFloat()) * sensitivity->GetFloat() : adjustedMx;
	}

	lastVelLength = velLength;
}