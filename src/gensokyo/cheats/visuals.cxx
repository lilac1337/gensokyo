#include "../../includes.hxx"

void gensokyo::cheats::visuals::alwaysShowCrosshair() noexcept {
	if (!options::visuals::alwaysShowCrosshair || !csgo::localPlayer)
		return;

	static ConVar* weaponDebugSpreadShow = csgo::interfaces::CVar->FindVar("weapon_debug_spread_show");

	weaponDebugSpreadShow->set_value((!csgo::localPlayer->m_bIsScoped()) * 3);
}

void gensokyo::cheats::visuals::nameTags() noexcept {
	if (!options::visuals::nameTagsEnabled)
		return;

	gui::renderer::texts.clear();

	static ConVar* clTeamidOverheadMode = csgo::interfaces::CVar->FindVar("cl_teamid_overhead_mode");

	clTeamidOverheadMode->set_value(1);

	for (int i = 1; i <= 64; ++i) {
		CEntity* entity = csgo::interfaces::EntityList->GetClientEntity(i);

		if (!entity)
			continue;

		if (entity->m_iPlayerState())
			continue;

		Vector3<float> textPosition{};
		player_info_t playerInfo{};
		Matrix3x4 bones[128];

		if (!entity->SetupBones(bones, 128, 0x7FF00, csgo::globals->curtime))
			continue;

		if (!csgo::interfaces::Engine->GetPlayerInfo(entity->GetIndex(), &playerInfo))
			continue;

		csgo::interfaces::DebugOverlay->ScreenPosition(bones[8].Origin() + Vector3<float>{ 0.f, 0.f, 15.f }, textPosition);

		D3DCOLOR red = D3DCOLOR_XRGB(220, 20, 60);
		D3DCOLOR blue = D3DCOLOR_XRGB(0, 191, 255);

		text textTest = { std::format("{} | {} HP", playerInfo.name, entity->m_iHealth()), textPosition.x, textPosition.y, (entity->m_iTeamNum() - 2) ? blue : red };

		gui::renderer::texts.push_back(textTest);
	}
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

