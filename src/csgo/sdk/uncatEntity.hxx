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
	NETVAR(m_iHealth, "CCSPlayer->m_iHealth", int)
	NETVAR(m_iPlayerState, "CCSPlayer->m_iPlayerState", int)
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

