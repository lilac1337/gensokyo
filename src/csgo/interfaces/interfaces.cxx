#include "../../includes.hxx"

namespace csgo::interfaces {
	void* GetInterface(const char* dllname, const char* interfacename) {
		tCreateInterface CreateInterface = (tCreateInterface)GetProcAddress(GetModuleHandle(dllname), "CreateInterface");

		int returnCode = 0;
		void* interfaces = CreateInterface(interfacename, &returnCode);

		if (interfaces == nullptr)
			gensokyo::util::logger::log(GNSKY_ERROR, std::format("couldn't find {} from {}!!!!", interfacename, dllname));

		gensokyo::util::logger::log(GNSKY_INFO, std::format("successfully got {} from {} at {:#x}.", interfacename, dllname, (int)interfaces));

		return interfaces;
	}

	void init() {
		CVar = static_cast<ICvar*>(GetInterface("vstdlib.dll", "VEngineCvar007"));
		InputSystem = static_cast<IInputSystem*>(GetInterface("inputsystem.dll", "InputSystemVersion001"));
		Client = static_cast<IClient*>(GetInterface("client.dll", "VClient018"));
		EntityList = static_cast<IEntityList*>(GetInterface("client.dll", "VClientEntityList003"));
		Engine = static_cast<CEngineClient*>(GetInterface("engine.dll", "VEngineClient014"));
		PlayerInfoManger = static_cast<CPlayerInfoManager*>(GetInterface("server.dll", "PlayerInfoManager002"));
		DebugOverlay = static_cast<IVDebugOverlay*>(GetInterface("engine.dll", "VDebugOverlay004"));

		//Panel = (IPanel*)GetInterface("vgui2.dll", "VGUI_Panel009");
		//Surface = (ISurface*)GetInterface("vguimatsurface.dll", "VGUI_Surface031");
		

		gensokyo::util::setupNetvars();


		csgo::globals = PlayerInfoManger->GetGlobalVars();

		gensokyo::util::logger::log(GNSKY_INFO, "all interfaces successfully found ^_^");
	}
}