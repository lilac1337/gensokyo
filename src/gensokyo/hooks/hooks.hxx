#pragma once

#include "../../includes.hxx"

namespace gensokyo::hooks {
	inline SafetyHookInline CreateMove;
	inline SafetyHookInline ApplyMouse;
	inline SafetyHookInline EndScene;
	inline SafetyHookMid determineKeySpeed;

	inline float keySpeed{};

	[[nodiscard]] bool __fastcall CreateMove_hk(void* _this, void* edx, float flInputSampleTime, CUserCmd* cmd) noexcept;
	void __fastcall ApplyMouse_hk(CInput* _this, void* edx, i32 nSlot, Vector3<float>& viewAngles, CUserCmd* cmd, float mouseX, float mouseY) noexcept;
	[[nodiscard]] HRESULT __stdcall EndScene_hk(IDirect3DDevice9* device) noexcept;

	void init() noexcept;
	void uninject() noexcept;
};