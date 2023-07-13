#pragma once

#include "../../includes.hxx"

//-----------------------------------------------------------------------------
//
// implementation of IVEngineHud
//
//-----------------------------------------------------------------------------

class CEngineClient : public IVEngineClient
{
public:
	CEngineClient();
	~CEngineClient();

	int		GetIntersectingSurfaces(
		const model_t* model,
		const Vector& vCenter,
		const float radius,
		const bool bOnlyVisible,
		SurfInfo* pInfos,
		const int nMaxInfos);

	Vector	GetLightForPoint(const Vector& pos, bool bClamp);
	Vector	GetLightForPointFast(const Vector& pos, bool bClamp);
	const char* ParseFile(const char* data, char* token, int maxlen);
	virtual bool CopyLocalFile(const char* source, const char* destination);
	void GetScreenSize(int& w, int& h);
	void ServerCmd(const char* szCmdString, bool bReliable);
	void ClientCmd(const char* szCmdString);
	void ClientCmd_Unrestricted(const char* szCmdString, bool fromConsoleOrKeybind = false);
	void ClientCmd_Unrestricted(const char* szCmdString, bool fromConsoleOrKeybind, int nUserSlot, bool bCheckValidSlot = true);
	void SetRestrictServerCommands(bool bRestrict);
	void SetRestrictClientCommands(bool bRestrict);
	bool GetPlayerInfo(int ent_num, player_info_t* pinfo);
	client_textmessage_t* TextMessageGet(const char* pName);
	bool Con_IsVisible(void);
	int GetLocalPlayer(void);
	float GetLastTimeStamp(void);
	virtual int GetLastAcknowledgedCommand(void);
	virtual int GetServerTick(void);
	const model_t* LoadModel(const char* pName, bool bProp);
	void UnloadModel(const model_t* model, bool bProp);
	CSentence* GetSentence(CAudioSource* pAudioSource);
	float GetSentenceLength(CAudioSource* pAudioSource);
	bool IsStreaming(CAudioSource* pAudioSource) const;

	// FIXME, move entirely to client .dll
	void GetViewAngles(Vector3<float>& va);
	void SetViewAngles(Vector3<float>& va);
	int GetMaxClients(void);
	void Key_Event(ButtonCode_t key, int down);
	const char* Key_LookupBinding(const char* pBinding);
	const char* Key_BindingForKey(ButtonCode_t code);
	void Key_SetBinding(ButtonCode_t code, const char* pBinding);
	void StartKeyTrapMode(void);
	bool CheckDoneKeyTrapping(ButtonCode_t& key);
	bool IsInGame(void);
	bool IsConnected(void);
	bool IsDrawingLoadingImage(void);
	void HideLoadingPlaque(void);
	void Con_NPrintf(int pos, const char* fmt, ...);
	void Con_NXPrintf(const struct con_nprint_s* info, const char* fmt, ...);
	IMaterial* TraceLineMaterialAndLighting(const Vector& start, const Vector& end,
		Vector& diffuseLightColor, Vector& baseColor);
	int		IsBoxVisible(const Vector& mins, const Vector& maxs);
	int		IsBoxInViewCluster(const Vector& mins, const Vector& maxs);

	void Sound_ExtraUpdate(void);
#if defined(_PS3)
	void Sound_ServerUpdateSoundsPS3(void);
#endif

	bool CullBox(const Vector& mins, const Vector& maxs);
	const char* GetGameDirectory(void);
	const VMatrix& WorldToScreenMatrix();
	const VMatrix& WorldToViewMatrix();

	// Loads a game lump off disk
	int		GameLumpVersion(int lumpId) const;
	int		GameLumpSize(int lumpId) const;
	bool	LoadGameLump(int lumpId, void* pBuffer, int size);

	// Returns the number of leaves in the level
	int		LevelLeafCount() const;
	virtual ISpatialQuery* GetBSPTreeQuery();

	// Convert texlight to gamma...
	virtual void LinearToGamma(float* linear, float* gamma);

	// Get the lightstyle value
	virtual float LightStyleValue(int style);
	virtual void DrawPortals();

	// Computes light due to dynamic lighting at a point
	// If the normal isn't specified, then it'll return the maximum lighting
	virtual void ComputeDynamicLighting(Vector const& pt, Vector const* pNormal, Vector& color);

	// Computes light due to dynamic lighting at a point
	// If the normal isn't specified, then it'll return the maximum lighting
	// If pBoxColors is specified (it's an array of 6), then it'll copy the light contribution at each box side.
	virtual void ComputeLighting(const Vector& pt, const Vector* pNormal, bool bClamp, Vector& color, Vector* pBoxColors);

	// Returns the color of the ambient light
	virtual void GetAmbientLightColor(Vector& color);

	// Returns the dx support level
	virtual int	GetDXSupportLevel();

	virtual bool SupportsHDR();
	virtual void Mat_Stub(IMaterialSystem* pMatSys);

	// menu display
	virtual void GetChapterName(char* pchBuff, int iMaxLength);
	virtual char const* GetLevelName(void);
	virtual char const* GetLevelNameShort(void);
	virtual char const* GetMapGroupName(void);
	virtual bool IsLevelMainMenuBackground(void);
	virtual void GetMainMenuBackgroundName(char* dest, int destlen);

	// Occlusion system control
	virtual void SetOcclusionParameters(const OcclusionParams_t& params);

	//-----------------------------------------------------------------------------
	// Purpose: Takes a trackerID and returns which player slot that user is in
	//			returns 0 if no player found with that ID
	//-----------------------------------------------------------------------------
	virtual int	GetPlayerForUserID(int userID);
#if !defined( NO_VOICE )
	virtual struct IVoiceTweak_s* GetVoiceTweakAPI(void);
	virtual void SetVoiceCasterID(u32 casterID);
#endif
	virtual void EngineStats_BeginFrame(void);
	virtual void EngineStats_EndFrame(void);
	virtual void FireEvents();
	virtual void ClearEvents();
	virtual void CheckPoint(const char* pName);
	virtual int GetLeavesArea(unsigned short* pLeaves, int nLeaves);
	virtual bool DoesBoxTouchAreaFrustum(const Vector& mins, const Vector& maxs, int iArea);
	virtual int GetFrustumList(void** pList, int listMax);
	virtual bool ShouldUseAreaFrustum(int area);

	// Sets the hearing origin
	virtual void SetAudioState(const AudioState_t& audioState);

	//-----------------------------------------------------------------------------
	//
	// Sentence API
	//
	//-----------------------------------------------------------------------------

	virtual int SentenceGroupPick(int groupIndex, char* name, int nameLen);
	virtual int SentenceGroupPickSequential(int groupIndex, char* name, int nameLen, int sentenceIndex, int reset);
	virtual int SentenceIndexFromName(const char* pSentenceName);
	virtual const char* SentenceNameFromIndex(int sentenceIndex);
	virtual int SentenceGroupIndexFromName(const char* pGroupName);
	virtual const char* SentenceGroupNameFromIndex(int groupIndex);
	virtual float SentenceLength(int sentenceIndex);
	virtual void DebugDrawPhysCollide(const CPhysCollide* pCollide, IMaterial* pMaterial, const void* transform, const color32& color);

	// Activates/deactivates an occluder...
	virtual void ActivateOccluder(int nOccluderIndex, bool bActive);
	virtual bool IsOccluded(int occlusionViewId, const Vector& vecAbsMins, const Vector& vecAbsMaxs);
	virtual int	GetOcclusionViewId() const;
	virtual void* SaveAllocMemory(size_t num, size_t size);
	virtual void SaveFreeMemory(void* pSaveMem);
	virtual INetChannelInfo* GetNetChannelInfo(void);
	virtual bool IsPlayingDemo(void);
	virtual bool IsRecordingDemo(void);
	virtual bool IsPlayingTimeDemo(void);
	virtual int  GetDemoRecordingTick(void);
	virtual int  GetDemoPlaybackTick(void);
	virtual int  GetDemoPlaybackStartTick(void);
	virtual float GetDemoPlaybackTimeScale(void);
	virtual int  GetDemoPlaybackTotalTicks(void);
	virtual CDemoPlaybackParameters_t const* GetDemoPlaybackParameters() ;
	virtual bool IsDemoSkipping(void) ;
	virtual int GetConnectionDataProtocol() const ;
	virtual bool EngineGotvSyncPacket(const CEngineGotvSyncPacket* pPkt) ;
	virtual void SetDemoImportantEventData(const KeyValues* pData) ;
	virtual bool IsPaused(void);
	virtual float GetTimescale(void) const;
	virtual bool IsTakingScreenshot(void);
	virtual void WriteScreenshot(const char* pFilename);
	virtual bool IsHLTV(void);
	virtual bool IsReplay(void);
	virtual void GetUILanguage(char* dest, int destlen);

	// Can skybox be seen from a particular point?
	virtual SkyboxVisibility_t IsSkyboxVisibleFromPoint(const Vector& vecPoint);

	virtual const char* GetMapEntitiesString();
	virtual bool		IsInEditMode(void);
	virtual bool		IsInCommentaryMode(void);
	virtual float		GetScreenAspectRatio(int viewportWidth, int viewportHeight);

	virtual unsigned int		GetEngineBuildNumber();
	virtual const char* GetProductVersionString();
	virtual void				GrabPreColorCorrectedFrame(int x, int y, int width, int height);
	virtual bool				IsHammerRunning() const;

	// Stuffs the cmd into the buffer & executes it immediately (vs ClientCmd() which executes it next frame)
	virtual void				ExecuteClientCmd(const char* szCmdString);

	virtual bool MapHasHDRLighting(void);
	virtual bool MapHasLightMapAlphaData(void);
	virtual int GetAppID();

	virtual void				SetOverlayBindProxy(int iOverlayID, void* pBindProxy);

	virtual bool				CopyFrameBufferToMaterial(const char* pMaterialName);

	// Matchmaking
	virtual void				ReadConfiguration(const int iController, const bool readDefault);

	virtual void SetAchievementMgr(IAchievementMgr* pAchievementMgr);
	virtual IAchievementMgr* GetAchievementMgr();

	virtual bool				MapLoadFailed(void);
	virtual void				SetMapLoadFailed(bool bState);

	virtual bool				IsLowViolence();
	virtual const char* GetMostRecentSaveGame(bool bEnsureExists);
	virtual void				SetMostRecentSaveGame(const char* lpszFilename);

	virtual void				StartXboxExitingProcess();

	virtual bool				IsSaveInProgress();
	virtual bool				IsAutoSaveDangerousInProgress();
	virtual bool				IsAutoSaveInProgress();

	virtual const char* GetSaveDirName(); // get a pointer to the path where saves should go (with a trailing slash already added)


	virtual u32				OnStorageDeviceAttached(int iController);
	virtual void				OnStorageDeviceDetached(int iController);

	virtual void				ResetDemoInterpolation(void);

	virtual bool		REMOVED_SteamRefreshLogin(const char* password, bool isSecure) { return false; }
	virtual bool		REMOVED_SteamProcessCall(bool& finished) { return false; }

	// For non-split screen games this will always be zero
	virtual int				GetActiveSplitScreenPlayerSlot();
	virtual int				SetActiveSplitScreenPlayerSlot(int slot);
	virtual bool			SetLocalPlayerIsResolvable(char const* pchContext, int nLine, bool bResolvable);
	virtual bool			IsLocalPlayerResolvable();

	virtual int				GetSplitScreenPlayer(int nSlot);
	virtual bool			IsSplitScreenActive();
	virtual bool			IsValidSplitScreenSlot(int nSlot);
	virtual int				FirstValidSplitScreenSlot(); // -1 == invalid
	virtual int				NextValidSplitScreenSlot(int nPreviousSlot); // -1 == invalid

	virtual ISPSharedMemory* GetSinglePlayerSharedMemorySpace(const char* handle, int ent_num = 32);

	// Computes an ambient cube that includes ALL dynamic lights
	virtual void ComputeLightingCube(const Vector& pt, bool bClamp, Vector* pBoxColors);

	//All callbacks have to be registered before demo recording begins. TODO: Macro'ize a way to do it at startup
	virtual void RegisterDemoCustomDataCallback(void* szCallbackSaveID, pfnDemoCustomDataCallback pCallback);
	virtual void RecordDemoCustomData(pfnDemoCustomDataCallback pCallback, const void* pData, size_t iDataLength);

	virtual void SetLeafFlag(int nLeafIndex, int nFlagBits);

	// you must call this once done modifying flags. Not super fast.
	virtual void RecalculateBSPLeafFlags(void);

	virtual bool DSPGetCurrentDASRoomNew(void);
	virtual bool DSPGetCurrentDASRoomChanged(void);
	virtual bool DSPGetCurrentDASRoomSkyAbove(void);
	virtual float DSPGetCurrentDASRoomSkyPercent(void);
	virtual void SetMixGroupOfCurrentMixer(const char* szgroupname, const char* szparam, float val, int setMixerType);
	virtual int GetMixLayerIndex(const char* szmixlayername);
	virtual void SetMixLayerLevel(int index, float level);
	virtual int GetMixGroupIndex(const char* pMixGroupName);
	virtual void SetMixLayerTriggerFactor(int nMixLayerIndex, int nMixGroupIndex, float flFactor);
	virtual void SetMixLayerTriggerFactor(const char* pMixLayerIndex, const char* pMixGroupIndex, float flFactor);


	virtual bool IsCreatingReslist();
	virtual bool IsCreatingXboxReslist();

	virtual void SetTimescale(float flTimescale);

	virtual void SetGamestatsData(CGamestatsData* pGamestatsData);
	virtual CGamestatsData* GetGamestatsData();

	virtual	const char* Key_LookupBindingEx(const char* pBinding, int iUserId = -1, int iStartCount = 0, BindingLookupOption_t nFlags = BINDINGLOOKUP_ALL);
	virtual int	Key_CodeForBinding(const char* pBinding, int iUserId = -1, int iStartCount = 0, BindingLookupOption_t nFlags = BINDINGLOOKUP_ALL);

	virtual void UpdateDAndELights(void);

	// Methods to get bug count for internal dev work stat tracking.
	// Will get the bug count and clear it every map transition
	virtual int			GetBugSubmissionCount() const;
	virtual void		ClearBugSubmissionCount();

	virtual bool	DoesLevelContainWater() const;
	virtual float	GetServerSimulationFrameTime() const;
	virtual void SolidMoved(IClientEntity* pSolidEnt, ICollideable* pSolidCollide, const Vector* pPrevAbsOrigin, bool accurateBboxTriggerChecks);
	virtual void TriggerMoved(IClientEntity* pTriggerEnt, bool accurateBboxTriggerChecks);
	virtual void ComputeLeavesConnected(const Vector& vecOrigin, int nCount, const int* pLeaves, bool* pIsConnected);

	virtual void	SetBlurFade(float scale);
	virtual bool	IsTransitioningToLoad();
	virtual void	SearchPathsChangedAfterInstall();
	virtual void ConfigureSystemLevel(int nCPULevel, int nGPULevel);
	virtual void SetConnectionPassword(char const* pchCurrentPW);
	virtual CSteamAPIContext* GetSteamAPIContext();
	virtual void SubmitStatRecord(char const* szMapName, u32 uiBlobVersion, u32 uiBlobSize, const void* pvBlob);

	virtual void ServerCmdKeyValues(KeyValues* pKeyValues);
	virtual void SendMessageToServer(INetMessage* pMessage, bool bForceReliable, bool bVoice) ;

	// global sound pitch scaling
	virtual void SetPitchScale(float flPitchScale);
	virtual float GetPitchScale(void);

	// Load/unload the SFM - used by Replay
	virtual bool LoadFilmmaker();
	virtual void UnloadFilmmaker();

	//paint stuff
	virtual bool SpherePaintSurface(const model_t* pModel, const Vector& vPosition, BYTE color, float flSphereRadius, float flPaintCoatPercent);
	virtual bool HasPaintmap();
	virtual void EnablePaintmapRender();
	virtual void SphereTracePaintSurface(const model_t* pModel, const Vector& vPosition, const Vector& vContactNormal, float flSphereRadius, void* surfColors);
	virtual void RemoveAllPaint();
	virtual void PaintAllSurfaces(BYTE color);
	virtual void RemovePaint(const model_t* pModel);

	virtual bool IsActiveApp();

	// is this client running inside the same process as an active server?
	virtual bool IsClientLocalToActiveServer();

#if defined( USE_SDL ) || defined ( OSX )
	virtual void GetMouseDelta(int& x, int& y, bool bIgnoreNextMouseDelta);
#endif	

	// Callback for LevelInit to tick the progress bar during time consuming operations
	virtual void TickProgressBar();
	// Returns the requested input context
	virtual void* GetInputContext(EngineInputContextId_t id);

	virtual void GetStartupImage(char* pOutBuffer, int nOutBufferSize);

	virtual bool IsUsingLocalNetworkBackdoor();

	virtual bool SaveGame(const char* pSaveFilename, bool bIsXSave, char* pOutName, int nOutNameSize, char* pOutComment, int nOutCommentSize);

	// Request 'generic' memory stats (returns a list of N named values; caller should assume this list will change over time)
	virtual int GetGenericMemoryStats(void** ppMemoryStats);

	// On exit from a map, this becomes true once all map-related assets are flushed from memory:
	virtual bool GameHasShutdownAndFlushedMemory();

	virtual void FinishContainerWrites(int iController);

	virtual void FinishAsyncSave();

	const char* GetModDirectory(void);

	virtual void AudioLanguageChanged();

	virtual void StartLoadingScreenForCommand(const char* command);

	virtual void StartLoadingScreenForKeyValues(KeyValues* keyValues);

	virtual bool SOSSetOpvarFloat(const char* pOpVarName, float flValue);
	virtual bool SOSGetOpvarFloat(const char* pOpVarName, float& flValue);

#if defined(_PS3)
	virtual void* GetHostStateWorldBrush(void);
	virtual bool PS3_IsUserRestrictedFromChat(void);
	virtual bool PS3_IsUserRestrictedFromOnline(void);
	virtual bool PS3_PendingInvitesFound(void);
	virtual void PS3_ShowInviteOverlay(void);

	virtual bool  bOverrideCSMConvars(void);
	virtual bool  bDrawWorldIntoCSM(void);
	virtual bool  bDrawStaticPropsIntoCSM(void);
	virtual float GetCSMMaxDist(void);
#endif

	virtual bool IsSubscribedMap(const char* pchMapName, bool bOnlyOnDisk);
	virtual bool IsFeaturedMap(const char* pchMapName, bool bOnlyOnDisk);

	virtual int GetClientVersion() const;

	virtual float GetSafeZoneXMin(void) const;

	virtual bool IsVoiceRecording() const ;
	virtual void ForceVoiceRecordOn() const ;
	virtual const char* AliasToCommandString(const char* szAliasName) ;
};
