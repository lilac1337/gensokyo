#pragma once

#include "../../includes.hxx"

class IEntityList;
class IClient;

namespace csgo::interfaces {
	inline ICvar* CVar = nullptr;
	inline IInputSystem* InputSystem = nullptr;
	//inline IPanel* Panel = nullptr;
	//inline ISurface* Surface = nullptr;
	inline IEntityList* EntityList = nullptr;
	inline IClient* Client = nullptr;
	inline CEngineClient* Engine = nullptr;
	inline CPlayerInfoManager* PlayerInfoManger = nullptr;
	inline IVDebugOverlay* DebugOverlay = nullptr;

	typedef void* (__cdecl* tCreateInterface)(const char* name, int* returnCode);

	void* GetInterface(const char* dllname, const char* interfacename);
	void init();
}