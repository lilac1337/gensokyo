#include "../../includes.hxx"

__forceinline float normalizeYaw(float yaw) {
	while (yaw > 180.0) yaw -= 360.0;
	while (yaw < -180.0) yaw += 360.0;
	return yaw;
};

[[nodiscard]] bool __fastcall gensokyo::hooks::CreateMove_hk(void* _this, void* edx, float flInputSampleTime, CUserCmd* cmd) noexcept {
	csgo::localPlayer = csgo::interfaces::EntityList->GetClientEntity(csgo::interfaces::Engine->GetLocalPlayer());

	const bool extraMouseSample = (!cmd->tick_count || !cmd->command_number);

	if (extraMouseSample && csgo::localPlayer->m_iPlayerState())
		return false;

	if (!extraMouseSample)
		cheats::createMove(cmd);

	CreateMove.fastcall<bool>(_this, edx, flInputSampleTime, cmd);
}

void __fastcall gensokyo::hooks::ApplyMouse_hk(CInput* _this, void* edx, i32 nSlot, Vector3<float>& viewAngles, CUserCmd* cmd, float mouseX, float mouseY) noexcept {
	if (csgo::localPlayer)
		cheats::applyMouse(_this, cmd, mouseX);

	return ApplyMouse.fastcall<void>(_this, edx, nSlot, &viewAngles, cmd, mouseX, mouseY);
}

[[nodiscard]] HRESULT __stdcall gensokyo::hooks::EndScene_hk(IDirect3DDevice9* device) noexcept {
	static const void* returnAddress = _ReturnAddress();

	const HRESULT result = EndScene.stdcall<HRESULT>(device);

	if (_ReturnAddress() == returnAddress) return result;

	if (GetAsyncKeyState(VK_INSERT) & 1) gensokyo::gui::handleOpenClose();

	const std::unique_ptr<D3DState> state = std::make_unique<D3DState>(device);

	gui::renderer::renderFrame();

	state->Restore();

	return result;
}

void gensokyo::hooks::init() noexcept {
	CreateMove = safetyhook::create_inline(reinterpret_cast<uptr*>(util::findPattern("client.dll", "55 8B EC 56 8D 75 04 8B 0E E8 ? ? ? ? 8B 0E")), CreateMove_hk);
	ApplyMouse = safetyhook::create_inline(reinterpret_cast<uptr*>(util::findPattern("client.dll", "55 8B EC 51 8B 55 08 56 8B F1 57")), ApplyMouse_hk);
	EndScene = safetyhook::create_inline(reinterpret_cast<uptr*>(util::vFunc(gui::renderer::device, 42)), EndScene_hk);
	determineKeySpeed = safetyhook::create_mid(util::findPattern("client.dll", "F3 0F 11 4E ? EB ? F3 0F 10 45"), [](safetyhook::Context& ctx) {
		if (ctx.edx)
			keySpeed = *reinterpret_cast<float*>(ctx.ebp - 4);
	});
}

void gensokyo::hooks::uninject() noexcept {
	CreateMove.reset();
	ApplyMouse.reset();
	EndScene.reset();
	determineKeySpeed.reset();
}