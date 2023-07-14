#pragma once

#include "../../includes.hxx"

class CEntity {
public:
	NETVAR(m_bSpotted, "CBaseEntity->m_bSpotted", bool)
	NETVAR(m_fFlags, "CBasePlayer->m_fFlags", int)
	NETVAR(m_vecVelocity, "CBasePlayer->m_vecVelocity[0]", Vector3<float>)
	NETVAR(m_bIsScoped, "CCSPlayer->m_bIsScoped", bool)
	NETVAR(m_angEyeAngles, "CCSPlayer->m_angEyeAngles[0]", Vector3<float>)
	NETVAR(m_lifeState, "CCSPlayer->m_lifeState", int)
	NETVAR(m_iObserverMode, "CCSPlayer->m_iObserverMode", int)
	NETVAR(m_iHealth, "CBasePlayer->m_iHealth", int)
	NETVAR(m_iPlayerState, "CCSPlayer->m_iPlayerState", int)
	NETVAR(m_iTeamNum, "CBaseEntity->m_iTeamNum", int)
	NETVAR(m_vecOrigin, "CBaseEntity->m_vecOrigin", Vector3<float>)
public:
	constexpr const Vector3<float>& GetAbsOrigin() noexcept {
		return gensokyo::util::callVfunc<const Vector3<float>&>(this, 10);
	}

	constexpr i32 GetHealth() noexcept {
		return gensokyo::util::callVfunc<i32>(this, 122);
	}

	constexpr i32 GetIndex() noexcept {
		return gensokyo::util::callVfunc<i32>(this + 0x8, 10);
	}

	constexpr bool SetupBones(Matrix3x4* out, i32 max, i32 mask, float currentTime) noexcept {
		return gensokyo::util::callVfunc<bool>(this + 0x4, 13, out, max, mask, currentTime);
	}
};

class IEntityList {
private:
	virtual void Unused_0() = 0;
	virtual void Unused_1() = 0;
	virtual void Unused_2() = 0;

public:
	virtual CEntity* GetClientEntity(int index) = 0;
	virtual int					NumberOfEntities(bool bIncludeNonNetworkable = false);

	virtual CEntity* GetClientUnknownFromHandle(CBaseHandle hEnt);
	virtual CEntity* GetClientNetworkableFromHandle(CBaseHandle hEnt);
	virtual CEntity* GetClientEntityFromHandle(CBaseHandle hEnt);
};

class IClient {
private:
	virtual void Unused_0() = 0;
	virtual void Unused_1() = 0;
	virtual void Unused_2() = 0;
	virtual void Unused_3() = 0;
	virtual void Unused_4() = 0;
	virtual void Unused_5() = 0;
	virtual void Unused_6() = 0;
	virtual void Unused_7() = 0;

public:
	virtual ClientClass* GetAllClass(void) = 0;
};

