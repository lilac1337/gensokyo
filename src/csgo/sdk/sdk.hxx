#pragma once

#include "../../includes.hxx"

//CATAGORIZE THESE

class IHandleEntity;
class CBaseHandle;

class CBaseHandle
{
	friend class CBaseEntityList;

public:

	CBaseHandle();
	//CBaseHandle(INVALID_EHANDLE_tag) {};
	CBaseHandle(const CBaseHandle& other);
	explicit CBaseHandle(IHandleEntity* pHandleObj);
	CBaseHandle(int iEntry, int iSerialNumber);

	// NOTE: The following constructor is not type-safe, and can allow creating an
	//       arbitrary CBaseHandle that doesn't necessarily point to an actual object.
	//
	// It is your responsibility to ensure that the target of the handle actually points
	// to the object you think it does.  Generally, the argument to this function should
	// have been obtained from CBaseHandle::ToInt() on a valid handle.
	static CBaseHandle UnsafeFromIndex(int index);

	void Init(int iEntry, int iSerialNumber);
	void Term();

	// Even if this returns true, Get() still can return return a non-null value.
	// This just tells if the handle has been initted with any values.
	bool IsValid() const;

	int GetEntryIndex() const;
	int GetSerialNumber() const;

	int ToInt() const;
	bool operator !=(const CBaseHandle& other) const;
	bool operator ==(const CBaseHandle& other) const;
	bool operator ==(const IHandleEntity* pEnt) const;
	bool operator !=(const IHandleEntity* pEnt) const;
	bool operator <(const CBaseHandle& other) const;
	bool operator <(const IHandleEntity* pEnt) const;

	// Assign a value to the handle.
	const CBaseHandle& operator=(const IHandleEntity* pEntity);
	const CBaseHandle& Set(const IHandleEntity* pEntity);

	// Use this to dereference the handle.
	// Note: this is implemented in game code (ehandle.h)
	IHandleEntity* Get() const;


protected:
	// The low NUM_SERIAL_BITS hold the index. If this value is less than MAX_EDICTS, then the entity is networkable.
	// The high NUM_SERIAL_NUM_BITS bits are the serial number.
	uint32_t m_Index;
};

enum class SendPropType : int
{
	INT = 0,
	FLOAT,
	VECTOR,
	VECTOR2D,
	STRING,
	ARRAY,
	DATATABLE,
	INT64,
	SENDPROPTYPEMAX
};

struct DataVariant
{
	union
	{
		float   Float;
		long    Int;
		char* String;
		void* Data;
		float   Vector[3];
		int64_t Int64;
	};

	SendPropType type;
};
struct RecvProp;
struct RecvTable
{
	RecvProp* props;
	int propsCount;
	void* decoder;
	char* tableName;
	bool initialized;
	bool inMainList;
};

struct RecvProp
{
	char* varName;
	SendPropType recvType;
	int flags;
	int stringBufferSize;
	bool insideArray;
	const void* extraData;
	RecvProp* arrayProp;
	void* arrayLengthProxyFn;
	void* proxyFn;
	void* dataTableProxyFn;
	RecvTable* dataTable;
	int offset;
	int elementStride;
	int elements;
	const char* parentArrayPropName;
};

class ClientClass
{
public:
	void* CreateClientClassFn;
	void* CreateEventFn;
	char* networkName;
	RecvTable* recvTable;
	ClientClass* next;
	int classID;
	const char* mapClassname;
};

class CGlobalVarsBase
{
public:

	CGlobalVarsBase(bool bIsClient);

	// This can be used to filter debug output or to catch the client or server in the act.
	bool IsClient() const;
	inline bool IsRemoteClient() const;

	// for encoding m_flSimulationTime, m_flAnimTime
	int GetNetworkBase(int nTick, int nEntity);

public:

	// Absolute time (per frame still - Use Plat_FloatTime() for a high precision real time 
	//  perf clock, but not that it doesn't obey host_timescale/host_framerate)
	float			realtime;
	// Absolute frame counter - continues to increase even if game is paused
	int				framecount;
	// Non-paused frametime
	float			absoluteframetime;
	float			absoluteframestarttimestddev;

	// Current time 
	//
	// On the client, this (along with tickcount) takes a different meaning based on what
	// piece of code you're in:
	// 
	//   - While receiving network packets (like in PreDataUpdate/PostDataUpdate and proxies),
	//     this is set to the SERVER TICKCOUNT for that packet. There is no interval between
	//     the server ticks.
	//     [server_current_Tick * tick_interval]
	//
	//   - While rendering, this is the exact client clock 
	//     [client_current_tick * tick_interval + interpolation_amount]
	//
	//   - During prediction, this is based on the client's current tick:
	//     [client_current_tick * tick_interval]
	float			curtime;

	// Time spent on last server or client frame (has nothing to do with think intervals)
	float			frametime;
	// current maxplayers setting
	int				maxClients;

	// Simulation ticks - does not increase when game is paused
	int				tickcount;

	// Simulation tick interval
	float			interval_per_tick;

	// interpolation amount ( client-only ) based on fraction of next tick which has elapsed
	float			interpolation_amount;
	int				simTicksThisFrame;

	int				network_protocol;

	// current saverestore data
	void* pSaveData;

private:
	// Set to true in client code.
	bool			m_bClient;
public:
	// true if we are a remote clinet (needs prediction & interpolation - server not on this machine) as opposed to split-screen or local
	bool			m_bRemoteClient;
private:
	// 100 (i.e., tickcount is rounded down to this base and then the "delta" from this base is networked
	int				nTimestampNetworkingBase;
	// 32 (entindex() % nTimestampRandomizeWindow ) is subtracted from gpGlobals->tickcount to set the networking basis, prevents
	//  all of the entities from forcing a new PackedEntity on the same tick (i.e., prevents them from getting lockstepped on this)
	int				nTimestampRandomizeWindow;

};

class CGlobalVars : public CGlobalVarsBase
{
public:

	CGlobalVars(bool bIsClient);

public:

	// Current map
	char		mapname;
	char		mapGroupName;
	int				mapversion;
	char		startspot;
	char	eLoadType;		// How the current map was loaded
	bool			bMapLoadFailed;	// Map has failed to load, we need to kick back to the main menu

	// game specific flags
	bool			deathmatch;
	bool			coop;
	bool			teamplay;
	// current maxentities
	int				maxEntities;

	int				serverCount;
	void* pEdicts;
};

class IPlayerInfo
{
public:
	// returns the players name (UTF-8 encoded)
	virtual const char* GetName() = 0;
	// returns the userid (slot number)
	virtual int		GetUserID() = 0;
	// returns the string of their network (i.e Steam) ID
	virtual const char* GetNetworkIDString() = 0;
	// returns the team the player is on
	virtual int GetTeamIndex() = 0;
	// changes the player to a new team (if the game dll logic allows it)
	virtual void ChangeTeam(int iTeamNum) = 0;
	// returns the number of kills this player has (exact meaning is mod dependent)
	virtual int	GetFragCount() = 0;
	// returns the number of deaths this player has (exact meaning is mod dependent)
	virtual int	GetDeathCount() = 0;
	// returns if this player slot is actually valid
	virtual bool IsConnected() = 0;
	// returns the armor/health of the player (exact meaning is mod dependent)
	virtual int	GetArmorValue() = 0;
};


class IPlayerInfoManager
{
public:
	virtual IPlayerInfo* GetPlayerInfo(void* pEdict) = 0;
	virtual CGlobalVars* GetGlobalVars() = 0;
};

class CPlayerInfoManager : public IPlayerInfoManager
{
public:
	virtual IPlayerInfo* GetPlayerInfo(void* pEdict);
	virtual CGlobalVars* GetGlobalVars();
};

template <class T, class I = int> class CUtlMemory {
public:
	T& operator[](int i) {
		return m_pMemory[i];
	};

	T* m_pMemory;
	int m_nAllocationCount;
	int m_nGrowSize;
};

class C_BaseCombatWeapon;
struct kbutton_t;

struct CameraThirdData_t;
struct CKeyboardKey;
class CRC32_t;
class bf_write;
class bf_read;

class CInput
{
	// Interface
public:
	CInput(void);
	~CInput(void);

	virtual		void		Init_All(void);
	virtual		void		Shutdown_All(void);
	virtual		int			GetButtonBits(int);
	virtual		void		CreateMove(int sequence_number, float input_sample_frametime, bool active);
	virtual		void		ExtraMouseSample(float frametime, bool active);
	virtual		bool		WriteUsercmdDeltaToBuffer(bf_write* buf, int from, int to, bool isnewcommand);
	virtual		void		EncodeUserCmdToBuffer(bf_write& buf, int slot);
	virtual		void		DecodeUserCmdFromBuffer(bf_read& buf, int slot);

	virtual		CUserCmd* GetUserCmd(int sequence_number);

	virtual		void		MakeWeaponSelection(C_BaseCombatWeapon* weapon);

	virtual		float		KeyState(kbutton_t* key);
	virtual		int			KeyEvent(int down, ButtonCode_t keynum, const char* pszCurrentBinding);
	virtual		kbutton_t* FindKey(const char* name);

	virtual		void		ControllerCommands(void);
	virtual		void		Joystick_Advanced(void);
	virtual		void		Joystick_SetSampleTime(float frametime);
	virtual		void		IN_SetSampleTime(float frametime);

	virtual		void		AccumulateMouse(void);
	virtual		void		ActivateMouse(void);
	virtual		void		DeactivateMouse(void);

	virtual		void		ClearStates(void);
	virtual		float		GetLookSpring(void);

	virtual		void		GetFullscreenMousePos(int* mx, int* my, int* unclampedx = NULL, int* unclampedy = NULL);
	virtual		void		SetFullscreenMousePos(int mx, int my);
	virtual		void		ResetMouse(void);

	//	virtual		bool		IsNoClipping( void );
	virtual		float		GetLastForwardMove(void);
	virtual		float		Joystick_GetForward(void);
	virtual		float		Joystick_GetSide(void);
	virtual		float		Joystick_GetPitch(void);
	virtual		float		Joystick_GetYaw(void);
	virtual		void		ClearInputButton(int bits);

	virtual		void		CAM_Think(void);
	virtual		int			CAM_IsThirdPerson(void);
	virtual		void		CAM_ToThirdPerson(void);
	virtual		void		CAM_ToFirstPerson(void);
	virtual		void		CAM_StartMouseMove(void);
	virtual		void		CAM_EndMouseMove(void);
	virtual		void		CAM_StartDistance(void);
	virtual		void		CAM_EndDistance(void);
	virtual		int			CAM_InterceptingMouse(void);

	// orthographic camera info
	virtual		void		CAM_ToOrthographic();
	virtual		bool		CAM_IsOrthographic() const;
	virtual		void		CAM_OrthographicSize(float& w, float& h) const;

	virtual		float		CAM_CapYaw(float fVal) { return fVal; }

#if defined( HL2_CLIENT_DLL )
	// IK back channel info
	virtual		void		AddIKGroundContactInfo(int entindex, float minheight, float maxheight);
#endif
	virtual		void		LevelInit(void);

	virtual		void		CAM_SetCameraThirdData(CameraThirdData_t* pCameraData, const Vector3<float>& vecCameraOffset);
	virtual		void		CAM_CameraThirdThink(void);

	virtual	bool		EnableJoystickMode();

	// Private Implementation

	// Implementation specific initialization
	void		Init_Camera(void);
	void		Init_Keyboard(void);
	void		Init_Mouse(void);
	void		Shutdown_Keyboard(void);
	// Add a named key to the list queryable by the engine
	void		AddKeyButton(const char* name, kbutton_t* pkb);
	// Mouse/keyboard movement input helpers
	void		ScaleMovements(CUserCmd* cmd);
	void		ComputeForwardMove(CUserCmd* cmd);
	void		ComputeUpwardMove(CUserCmd* cmd);
	void		ComputeSideMove(CUserCmd* cmd);
	void		AdjustAngles(float frametime);
	void		ClampAngles(Vector3<float>& viewangles);
	void		AdjustPitch(float speed, Vector3<float>& viewangles);
	virtual void AdjustYaw(float speed, Vector3<float>& viewangles);
	float		DetermineKeySpeed(float frametime);
	void		GetAccumulatedMouseDeltasAndResetAccumulators(float* mx, float* my);
	void		GetMouseDelta(float inmousex, float inmousey, float* pOutMouseX, float* pOutMouseY);
	void		ScaleMouse(float* x, float* y);
	void		ApplyMouse(Vector3<float>& viewangles, CUserCmd* cmd, float mouse_x, float mouse_y);
	void		MouseMove(CUserCmd* cmd);

	// Joystick  movement input helpers
	void		ControllerMove(float frametime, CUserCmd* cmd);
	void		JoyStickMove(float frametime, CUserCmd* cmd);
	float		ScaleAxisValue(const float axisValue, const float axisThreshold);
	virtual float JoyStickAdjustYaw(float flSpeed) { return flSpeed; }

	// Call this to get the cursor position. The call will be logged in the VCR file if there is one.
	void		GetMousePos(int& x, int& y);
	void		SetMousePos(int x, int y);
	void		GetWindowCenter(int& x, int& y);
	// Called once per frame to allow convar overrides to acceleration settings when mouse is active
	void		CheckMouseAcclerationVars();

	void		ValidateUserCmd(CUserCmd* usercmd, int sequence_number);

	// Private Data

	typedef struct
	{
		unsigned int AxisFlags;
		unsigned int AxisMap;
		unsigned int ControlMap;
	} joy_axis_t;

	void		DescribeJoystickAxis(char const* axis, joy_axis_t* mapping);
	char const* DescribeAxis(int index);

	enum
	{
		GAME_AXIS_NONE = 0,
		GAME_AXIS_FORWARD,
		GAME_AXIS_PITCH,
		GAME_AXIS_SIDE,
		GAME_AXIS_YAW,
		MAX_GAME_AXES
	};

	enum
	{
		CAM_COMMAND_NONE = 0,
		CAM_COMMAND_TOTHIRDPERSON = 1,
		CAM_COMMAND_TOFIRSTPERSON = 2
	};

	enum
	{
		MOUSE_ACCEL_THRESHHOLD1 = 0,	// if mouse moves > this many mickey's double it
		MOUSE_ACCEL_THRESHHOLD2,		// if mouse moves > this many mickey's double it a second time
		MOUSE_SPEED_FACTOR,				// 0 = disabled, 1 = threshold 1 enabled, 2 = threshold 2 enabled

		NUM_MOUSE_PARAMS,
	};

	// Has the mouse been initialized?
	bool		m_fMouseInitialized;
	// Is the mosue active?
	bool		m_fMouseActive;
	// Has the joystick advanced initialization been run?
	bool		m_fJoystickAdvancedInit;
	// Used to support hotplugging by reinitializing the advanced joystick system when we toggle between some/none joysticks.
	bool		m_fHadJoysticks;

	// Accumulated mouse deltas
	float		m_flAccumulatedMouseXMovement;
	float		m_flAccumulatedMouseYMovement;
	float		m_flPreviousMouseXPosition;
	float		m_flPreviousMouseYPosition;
	float		m_flRemainingJoystickSampleTime;
	float		m_flKeyboardSampleTime;

	// Flag to restore systemparameters when exiting
	bool		m_fRestoreSPI;
	// Original mouse parameters
	int			m_rgOrigMouseParms[NUM_MOUSE_PARAMS];
	// Current mouse parameters.
	int			m_rgNewMouseParms[NUM_MOUSE_PARAMS];
	bool		m_rgCheckMouseParam[NUM_MOUSE_PARAMS];
	// Are the parameters valid
	bool		m_fMouseParmsValid;
	// Joystick Axis data
	joy_axis_t m_rgAxes[64];
	// List of queryable keys
	CKeyboardKey* m_pKeys;

	// Is the 3rd person camera using the mouse?
	bool		m_fCameraInterceptingMouse;
	// Are we in 3rd person view?
	bool		m_fCameraInThirdPerson;
	// Should we move view along with mouse?
	bool		m_fCameraMovingWithMouse;


	// Is the camera in distance moving mode?
	bool		m_fCameraDistanceMove;
	// Old and current mouse position readings.
	int			m_nCameraOldX;
	int			m_nCameraOldY;
	int			m_nCameraX;
	int			m_nCameraY;

	// orthographic camera settings
	bool		m_CameraIsOrthographic;

	Vector3<float>		m_angPreviousViewAngles;

	float		m_flLastForwardMove;

	float m_flPreviousJoystickForward;
	float m_flPreviousJoystickSide;
	float m_flPreviousJoystickPitch;
	float m_flPreviousJoystickYaw;

	class CVerifiedUserCmd
	{
	public:
		CUserCmd	m_cmd;
		//CRC32_t		m_crc;
	};

	CUserCmd* m_pCommands;
	CVerifiedUserCmd* m_pVerifiedCommands;

	CameraThirdData_t* m_pCameraThirdData;

	// Set until polled by CreateMove and cleared
	//CHandle< C_BaseCombatWeapon > m_hSelectedWeapon;

#if defined( HL2_CLIENT_DLL )
	CUtlVector< CEntityGroundContact > m_EntityGroundContact;
#endif
};

class ConVar {
public:
	void set_value(const char* value) {
		using original_fn = void(__thiscall*)(ConVar*, const char*);
		return (*(original_fn**)this)[14](this, value);
	}
	void set_value(float value) {
		using original_fn = void(__thiscall*)(ConVar*, float);
		return (*(original_fn**)this)[15](this, value);
	}
	void set_value(int value) {
		using original_fn = void(__thiscall*)(ConVar*, int);
		return (*(original_fn**)this)[16](this, value);
	}
	void set_value(bool value) {
		using original_fn = void(__thiscall*)(ConVar*, int);
		return (*(original_fn**)this)[16](this, static_cast<int>(value));
	}
private:
	char pad_0x0018[0x4];
public:
	ConVar* pNext;                //0x04
	bool                            bRegistered;        //0x08
	const char* szName;                //0x0C
	const char* szHelpString;        //0x10
	int                                nFlags;                //0x14
	void* pCallback;            //0x18
	ConVar* pParent;            //0x1C
	const char* szDefaultValue;        //0x20
	char* szString;            //0x24
	int                                iStringLength;        //0x28
	float                            flValue;            //0x2C
	int                                iValue;                //0x30
	bool                            bHasMin;            //0x34
	float                            flMinValue;            //0x38
	bool                            bHasMax;            //0x3C
	float                            flMaxValue;            //0x40
	void* fnChangeCallbacks;    //0x44

	float GetFloat() // idx @ 11
	{
		std::uint32_t uXored = *reinterpret_cast<std::uint32_t*>(&pParent->flValue) ^ reinterpret_cast<std::uint32_t>(this);
		return *reinterpret_cast<float*>(&uXored);
	}

	int GetInt() // idx @ 12
	{
		return static_cast<int>(pParent->iValue ^ reinterpret_cast<int>(this));
	}
};