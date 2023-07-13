#include "../../includes.hxx"

void gensokyo::cheats::visuals::alwaysShowCrosshair() noexcept {
	if (!options::visuals::alwaysShowCrosshair || !csgo::localPlayer)
		return;

	static ConVar* weaponDebugSpreadShow = csgo::interfaces::CVar->FindVar("weapon_debug_spread_show");

	weaponDebugSpreadShow->set_value((!csgo::localPlayer->m_bIsScoped()) * 3);
}


void gensokyo::cheats::visuals::radar() noexcept {
	if (!options::visuals::radarEnabled)
		return;

	for (int i = 1; i <= 64; ++i) {
		CEntity* entity = csgo::interfaces::EntityList->GetClientEntity(i);

		if (!entity)
			continue;

		entity->m_bSpotted() = true;
	}
}
