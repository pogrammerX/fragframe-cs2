#pragma once

// used: schema field
#include "schema.h"
#include "Handle.h"

using GameTime_t = std::float_t;
using GameTick_t = std::int32_t;

#define TICK_INTERVAL 0.015625f
#define TIME_TO_TICKS(TIME) (static_cast<int>(0.5f + static_cast<float>(TIME) / TICK_INTERVAL))
#define TICKS_TO_TIME(TICKS) (TICK_INTERVAL * static_cast<float>(TICKS))
#define ROUND_TO_TICKS(TIME) (TICK_INTERVAL * TIME_TO_TICKS(TIME))
#define TICK_NEVER_THINK (-1)

class CEntityInstance;

class CUserCMD;

class CPlayer_MovementServices
{
public:
	SCHEMA_ADD_FIELD(int32_t, m_nImpulse, "CPlayer_MovementServices->m_nImpulse");
	SCHEMA_ADD_FIELD(uint64_t, m_nQueuedButtonDownMask, "CPlayer_MovementServices->m_nQueuedButtonDownMask");
	SCHEMA_ADD_FIELD(uint64_t, m_nQueuedButtonChangeMask, "CPlayer_MovementServices->m_nQueuedButtonChangeMask");
	SCHEMA_ADD_FIELD(uint64_t, m_nButtonDoublePressed, "CPlayer_MovementServices->m_nButtonDoublePressed");
	SCHEMA_ADD_FIELD(uint32_t[64], m_pButtonPressedCmdNumber, "CPlayer_MovementServices->m_pButtonPressedCmdNumber");
	SCHEMA_ADD_FIELD(uint32_t, m_nLastCommandNumberProcessed, "CPlayer_MovementServices->m_nLastCommandNumberProcessed");
	SCHEMA_ADD_FIELD(uint64_t, m_nToggleButtonDownMask, "CPlayer_MovementServices->m_nToggleButtonDownMask");
	SCHEMA_ADD_FIELD(float, m_flMaxspeed, "CPlayer_MovementServices->m_flMaxspeed");
	SCHEMA_ADD_FIELD(float[4], m_arrForceSubtickMoveWhen, "CPlayer_MovementServices->m_arrForceSubtickMoveWhen");
	SCHEMA_ADD_FIELD(float, m_flForwardMove, "CPlayer_MovementServices->m_flForwardMove");
	SCHEMA_ADD_FIELD(float, m_flLeftMove, "CPlayer_MovementServices->m_flLeftMove");
	SCHEMA_ADD_FIELD(float, m_flUpMove, "CPlayer_MovementServices->m_flUpMove");
	SCHEMA_ADD_FIELD(Vector, m_vecLastMovementImpulses, "CPlayer_MovementServices->m_vecLastMovementImpulses");
};

class CPlayer_MovementServices_Humanoid : public CPlayer_MovementServices
{
public:
	SCHEMA_ADD_FIELD(bool, m_bDucking, "CPlayer_MovementServices_Humanoid->m_bDucking");
	SCHEMA_ADD_FIELD(bool, m_bDucked, "CPlayer_MovementServices_Humanoid->m_bDucked");

	SCHEMA_ADD_FIELD(float_t, m_flSurfaceFriction, "CPlayer_MovementServices_Humanoid->m_flSurfaceFriction");
};

class CCSPlayer_MovementServices : public CPlayer_MovementServices_Humanoid
{
public:
	void ParachuteAccelerate(Vector& vecOutVelocity, float& flStamina, float& flFriction, Vector vecMove, float flFrameTime);

public:
	SCHEMA_ADD_FIELD(float_t, m_flDuckAmount, "CCSPlayer_MovementServices->m_flDuckAmount");
	SCHEMA_ADD_FIELD(float_t, m_flStamina, "CCSPlayer_MovementServices->m_flStamina");
};

class C_CSGameRules
{
public:
	SCHEMA_ADD_FIELD(bool, m_bFreezePeriod, "C_CSGameRules->m_bFreezePeriod");
	SCHEMA_ADD_FIELD(bool, m_bWarmupPeriod, "C_CSGameRules->m_bWarmupPeriod");
	SCHEMA_ADD_FIELD(bool, m_bTeamIntroPeriod, "C_CSGameRules->m_bTeamIntroPeriod");
	SCHEMA_ADD_FIELD(bool, m_bCTTimeOutActive, "C_CSGameRules->m_bCTTimeOutActive");
	SCHEMA_ADD_FIELD(bool, m_bTerroristTimeOutActive, "C_CSGameRules->m_bTerroristTimeOutActive");
	SCHEMA_ADD_FIELD(float, m_flTerroristTimeOutRemaining, "C_CSGameRules->m_flTerroristTimeOutRemaining");
	SCHEMA_ADD_FIELD(float, m_flCTTimeOutRemaining, "C_CSGameRules->m_flCTTimeOutRemaining");
	SCHEMA_ADD_FIELD(int, m_nTerroristTimeOuts, "C_CSGameRules->m_nTerroristTimeOuts");
	SCHEMA_ADD_FIELD(int, m_nCTTimeOuts, "C_CSGameRules->m_nCTTimeOuts");
	SCHEMA_ADD_FIELD(bool, m_bIsValveDS, "C_CSGameRules->m_bIsValveDS");
	SCHEMA_ADD_FIELD(bool, m_bHasMatchStarted, "C_CSGameRules->m_bHasMatchStarted");
	SCHEMA_ADD_FIELD(bool, m_bServerPaused, "C_CSGameRules->m_bServerPaused");
	SCHEMA_ADD_FIELD(bool, m_bBombPlanted, "C_CSGameRules->m_bBombPlanted");
	SCHEMA_ADD_FIELD(float, m_fRoundStartTime, "C_CSGameRules->m_fRoundStartTime");
	SCHEMA_ADD_FIELD(int, m_iRoundTime, "C_CSGameRules->m_iRoundTime");
	SCHEMA_ADD_FIELD(int, m_totalRoundsPlayed, "C_CSGameRules->m_totalRoundsPlayed");
	SCHEMA_ADD_FIELD(float, m_timeUntilNextPhaseStarts, "C_CSGameRules->m_timeUntilNextPhaseStarts");

	int GetFreezeTimeLeft();
	int GetRoundTimeLeft();
	int GetRound();
};

class IGlobalVars
{
public:
	float m_flRealTime; //0x0000
	int32_t m_nFrameCount; //0x0004
	float m_flFrameTime; //0x0008
	float m_flFrameTime2; //0x000C
	int32_t m_nMaxClients; //0x0010
	MEM_PAD(0x1C);
	float m_flIntervalPerSubTick; //0x0030
	float m_flCurrentTime; //0x0034
	float m_flCurrentTime2; //0x0038
	MEM_PAD(0xC);
	int32_t m_nTickCount; //0x0048
	MEM_PAD(0x8);
	int m_nSomeInt;
};

class CEntityIdentity
{
public:
	CS_CLASS_NO_INITIALIZER(CEntityIdentity);

	// @note: handle index is not entity index
	[[nodiscard]] inline std::add_lvalue_reference_t<std::uint32_t> GetIndex() {
		static const std::uint32_t uOffset = 0x10;
		return *reinterpret_cast<std::add_pointer_t<std::uint32_t>>(reinterpret_cast<std::uint8_t*>(this) + (uOffset));
	};
	SCHEMA_ADD_FIELD(const char*, m_designerName, "CEntityIdentity->m_designerName");
	SCHEMA_ADD_FIELD(std::uint32_t, m_flags, "CEntityIdentity->m_flags");

	[[nodiscard]] bool IsValid()
	{
		return GetIndex() != INVALID_EHANDLE_INDEX;
	}

	[[nodiscard]] int GetEntryIndex()
	{
		if (!IsValid())
			return ENT_ENTRY_MASK;

		return GetIndex() & ENT_ENTRY_MASK;
	}

	[[nodiscard]] int GetSerialNumber()
	{
		return GetIndex() >> NUM_SERIAL_NUM_SHIFT_BITS;
	}

	CEntityInstance* pInstance; // 0x00
};

class CEntityInstance
{
public:
	CS_CLASS_NO_INITIALIZER(CEntityInstance);

	void GetSchemaClassInfo(SchemaClassInfoData_t** pReturn)
	{
		return MEM::CallVFunc<void, 38U>(this, pReturn);
	}

	[[nodiscard]] CBaseHandle m_hEntityHandle()
	{
		CEntityIdentity* pIdentity = m_pEntity();
		if (pIdentity == nullptr)
			return CBaseHandle();

		return CBaseHandle(pIdentity->GetEntryIndex(), pIdentity->GetSerialNumber() - (pIdentity->m_flags() & 1));
	}

	SCHEMA_ADD_FIELD(CEntityIdentity*, m_pEntity, "CEntityInstance->m_pEntity");
};

class CCollisionProperty
{
public:
	CS_CLASS_NO_INITIALIZER(CCollisionProperty);

	SCHEMA_ADD_FIELD(Vector, m_vecMins, "CCollisionProperty->m_vecMins");
	SCHEMA_ADD_FIELD(Vector, m_vecMaxs, "CCollisionProperty->m_vecMaxs");

	std::uint16_t CollisionMask() {
		return *reinterpret_cast<std::uint16_t*>((uintptr_t)(this) + 0x38); // TODO: is this in schema?
	}
};

class CModel // TODO: remove fully, unused
{
private:
	MEM_PAD(0x70);                    // 0x0000
	int32_t m_iRendermeshCount;        // 0x0070
	MEM_PAD(0x4);                    // 0x0074
	void* m_pRenderMeshes;    // 0x0078
};

using CUtlSymLargeId = int*;

class CUtlSymbolLarge
{
	inline static const CUtlSymLargeId UTL_INVALID_SYMBOL_LARGE = reinterpret_cast<CUtlSymLargeId>(-1);

public:
	CUtlSymbolLarge() { id_ = UTL_INVALID_SYMBOL_LARGE; }

	explicit CUtlSymbolLarge(CUtlSymLargeId id) { id_ = id; }

	CUtlSymbolLarge(CUtlSymbolLarge const& sym) { id_ = sym.id_; }

	CUtlSymbolLarge& operator=(CUtlSymbolLarge const& src)
	{
		id_ = src.id_;

		return *this;
	}

	bool operator==(CUtlSymbolLarge const& src) const { return id_ == src.id_; }

	bool operator==(CUtlSymLargeId const& src) const { return id_ == src; }

	bool operator!=(CUtlSymbolLarge const& src) const { return id_ != src.id_; }

	bool operator!=(CUtlSymLargeId const& src) const { return id_ != src; }

	explicit operator CUtlSymLargeId() const { return id_; }

	[[nodiscard]] const char* String() const
	{
		if (id_ == UTL_INVALID_SYMBOL_LARGE)
			return "";

		return string_;
	}

	[[nodiscard]] bool IsValid() const
	{
		return id_ != UTL_INVALID_SYMBOL_LARGE;
	}

private:
	union
	{
		CUtlSymLargeId id_;
		const char* string_ = "";
	};
};

class CModelState
{
public:
	std::uint8_t padding_0[0x80];
	void* bones;
	std::uint8_t padding_1[0x18];
	CModel* modelHandle; // I think its just 8 bytes of pointer no? Well, im too lazy to look, we dont use this class anyway, too bad!
	CUtlSymbolLarge modelName;
};

class CSkeletonInstance
{
public:
	MEM_PAD(0x1CC); //0x0000
	int m_nBones; //0x01BC 
	MEM_PAD(0x18); //0x01C0
	int m_nMask; //0x01D8 
	MEM_PAD(0x4); //0x01DC
	void* m_pBoneCache;//0x1E0

	SCHEMA_ADD_FIELD(CModelState, m_modelState, "CSkeletonInstance->m_modelState");
};
class CGameSceneNode {
public:
	CS_CLASS_NO_INITIALIZER(CGameSceneNode);

	SCHEMA_ADD_FIELD(CEntityInstance*, m_pOwner, "CGameSceneNode->m_pOwner");
	SCHEMA_ADD_FIELD(CGameSceneNode*, m_pParent, "CGameSceneNode->m_pParent");
	SCHEMA_ADD_FIELD(CGameSceneNode*, m_pChild, "CGameSceneNode->m_pChild");
	SCHEMA_ADD_FIELD(CGameSceneNode*, m_pNextSibling, "CGameSceneNode->m_pNextSibling");
	SCHEMA_ADD_FIELD(CBaseHandle, m_hParent, "CGameSceneNode->m_hParent");
	SCHEMA_ADD_FIELD(Vector, m_vecOrigin, "CGameSceneNode->m_vecOrigin");
	SCHEMA_ADD_FIELD(QAngle, m_angRotation, "CGameSceneNode->m_angRotation");
	SCHEMA_ADD_FIELD(float, m_flScale, "CGameSceneNode->m_flScale");
	SCHEMA_ADD_FIELD(Vector, m_vecAbsOrigin, "CGameSceneNode->m_vecAbsOrigin");
	SCHEMA_ADD_FIELD(QAngle, m_angAbsRotation, "CGameSceneNode->m_angAbsRotation");
	SCHEMA_ADD_FIELD(float, m_flAbsScale, "CGameSceneNode->m_flAbsScale");
	SCHEMA_ADD_FIELD(int, m_nParentAttachmentOrBone, "CGameSceneNode->m_nParentAttachmentOrBone");
	SCHEMA_ADD_FIELD(bool, m_bDebugAbsOriginChanges, "CGameSceneNode->m_bDebugAbsOriginChanges");
	SCHEMA_ADD_FIELD(bool, m_bDormant, "CGameSceneNode->m_bDormant");
	SCHEMA_ADD_FIELD(bool, m_bForceParentToBeNetworked, "CGameSceneNode->m_bForceParentToBeNetworked");
	SCHEMA_ADD_FIELD(uint8_t, m_nHierarchicalDepth, "CGameSceneNode->m_nHierarchicalDepth");
	SCHEMA_ADD_FIELD(uint8_t, m_nHierarchyType, "CGameSceneNode->m_nHierarchyType");
	SCHEMA_ADD_FIELD(uint8_t, m_nDoNotSetAnimTimeInInvalidatePhysicsCount, "CGameSceneNode->m_nDoNotSetAnimTimeInInvalidatePhysicsCount");
	SCHEMA_ADD_FIELD(float, m_flZOffset, "CGameSceneNode->m_flZOffset");
	SCHEMA_ADD_FIELD(float, m_flClientLocalScale, "CGameSceneNode->m_flClientLocalScale");
	SCHEMA_ADD_FIELD(Vector, m_vRenderOrigin, "CGameSceneNode->m_vRenderOrigin");

	CSkeletonInstance* m_pSkeletonInstance()
	{
		return MEM::CallVFunc<CSkeletonInstance*, 8>(this);
	}
};

typedef enum ETeam : int {
	T = 2,
	CT = 3
};

class CBaseAnimGraphController
{
public:
	CBaseAnimGraphController() {}
	CBaseAnimGraphController(CBaseAnimGraphController&&) = delete; 
	CBaseAnimGraphController(const CBaseAnimGraphController&) = delete; 
	CBaseAnimGraphController& operator=(CBaseAnimGraphController&&) = delete; 
	CBaseAnimGraphController& operator=(const CBaseAnimGraphController&) = delete;;


	SCHEMA_ADD_FIELD(bool, m_bSequenceFinished, "CBaseAnimGraphController->m_bSequenceFinished");
	SCHEMA_ADD_FIELD(bool, m_bNetworkedAnimationInputsChanged, "CBaseAnimGraphController->m_bNetworkedAnimationInputsChanged");
	SCHEMA_ADD_FIELD(bool, m_bNetworkedSequenceChanged, "CBaseAnimGraphController->m_bNetworkedSequenceChanged");
	SCHEMA_ADD_FIELD(bool, m_bLastUpdateSkipped, "CBaseAnimGraphController->m_bLastUpdateSkipped");

	SCHEMA_ADD_FIELD(int, m_hSequence, "CBaseAnimGraphController->m_hSequence");
	SCHEMA_ADD_FIELD(int, m_flSeqFixedCycle, "CBaseAnimGraphController->m_flSeqFixedCycle");

	SCHEMA_ADD_FIELD(float, m_flPlaybackRate, "CBaseAnimGraphController->m_flPlaybackRate");
	SCHEMA_ADD_FIELD(float, m_flSoundSyncTime, "CBaseAnimGraphController->m_flSoundSyncTime");

	SCHEMA_ADD_FIELD(GameTime_t, m_flPrevAnimUpdateTime, "CBaseAnimGraphController->m_flPrevAnimUpdateTime");
	SCHEMA_ADD_FIELD(GameTime_t, m_flSeqStartTime, "CBaseAnimGraphController->m_flSeqStartTime");

	SCHEMA_ADD_FIELD(uintptr_t, m_nActiveIKChainMask, "CBaseAnimGraphController->m_nActiveIKChainMask");
};

class CBodyComponent
{
public:
	CS_CLASS_NO_INITIALIZER(CBodyComponent);

	SCHEMA_ADD_FIELD(CGameSceneNode*, m_pSceneNode, "CBodyComponent->m_pSceneNode");
	SCHEMA_ADD_PFIELD(CBaseAnimGraphController, m_animationController, "CBodyComponentBaseAnimGraph->m_animationController");
};

class C_BaseEntity : public CEntityInstance
{
public:
	CS_CLASS_NO_INITIALIZER(C_BaseEntity);

	SCHEMA_ADD_FIELD(CBodyComponent*, m_CBodyComponent, "C_BaseEntity->m_CBodyComponent");
	SCHEMA_ADD_FIELD(float, m_flSimulationTime, "C_BaseEntity->m_flSimulationTime");
	SCHEMA_ADD_FIELD(uint64_t, m_nSubclassID, "C_BaseEntity->m_nSubclassID");
	SCHEMA_ADD_FIELD(float, m_flGravityScale, "C_BaseEntity->m_flGravityScale");
	SCHEMA_ADD_FIELD(CBaseHandle, m_hGroundEntity, "C_BaseEntity->m_hGroundEntity");
	SCHEMA_ADD_FIELD(float, m_flAnimTime, "C_BaseEntity->m_flAnimTime");
	SCHEMA_ADD_FIELD_OFFSET(void*, m_VData, "C_BaseEntity->m_nSubclassID", 0x8);

	[[nodiscard]] bool IsBasePlayerController()
	{
		SchemaClassInfoData_t* pClassInfo;
		GetSchemaClassInfo(&pClassInfo);
		if (pClassInfo == nullptr)
			return false;

		return FNV1A::Hash(pClassInfo->szName) == FNV1A::HashConst("CCSPlayerController");
	}

	[[nodiscard]] bool IsPlayerPawn()
	{
		SchemaClassInfoData_t* pClassInfo;
		GetSchemaClassInfo(&pClassInfo);
		if (pClassInfo == nullptr)
			return false;

		return FNV1A::Hash(pClassInfo->szName) == FNV1A::HashConst("C_CSPlayerPawn");
	}

	SCHEMA_ADD_FIELD(CGameSceneNode*, m_pGameSceneNode, "C_BaseEntity->m_pGameSceneNode");
	SCHEMA_ADD_FIELD(CCollisionProperty*, m_pCollision, "C_BaseEntity->m_pCollision");
	SCHEMA_ADD_FIELD(std::uint8_t, m_iTeamNum, "C_BaseEntity->m_iTeamNum");
	SCHEMA_ADD_FIELD(CBaseHandle, m_hOwnerEntity, "C_BaseEntity->m_hOwnerEntity");
	SCHEMA_ADD_FIELD(Vector, m_vecBaseVelocity, "C_BaseEntity->m_vecBaseVelocity");
	SCHEMA_ADD_FIELD(Vector, m_vecAbsVelocity, "C_BaseEntity->m_vecAbsVelocity");
	SCHEMA_ADD_FIELD(Vector, m_vecVelocity, "C_BaseEntity->m_vecVelocity");
	SCHEMA_ADD_FIELD(bool, m_bTakesDamage, "C_BaseEntity->m_bTakesDamage");
	SCHEMA_ADD_FIELD(std::uint32_t, m_fFlags, "C_BaseEntity->m_fFlags");
	SCHEMA_ADD_FIELD(std::int32_t, m_iEFlags, "C_BaseEntity->m_iEFlags");
	SCHEMA_ADD_FIELD(std::int32_t, m_MoveType, "C_BaseEntity->m_MoveType");
	SCHEMA_ADD_FIELD(std::int32_t, m_nActualMoveType, "C_BaseEntity->m_nActualMoveType");
	SCHEMA_ADD_FIELD(std::uint8_t, m_lifeState, "C_BaseEntity->m_lifeState");
	SCHEMA_ADD_FIELD(std::int32_t, m_iHealth, "C_BaseEntity->m_iHealth");
	SCHEMA_ADD_FIELD(std::int32_t, m_iMaxHealth, "C_BaseEntity->m_iMaxHealth");
	SCHEMA_ADD_FIELD(float, m_flWaterLevel, "C_BaseEntity->m_flWaterLevel");
	SCHEMA_ADD_FIELD(int, m_nCreationTick, "C_BaseEntity->m_nCreationTick");
	SCHEMA_ADD_FIELD(GameTime_t, m_flCreateTime, "C_BaseEntity->m_flCreateTime");
	SCHEMA_ADD_FIELD(bool, m_bPredictable, "C_BaseEntity->m_bPredictable");
	// SCHEMA_ADD_FIELD_OFFSET(CCSWeaponBaseVData*, m_VData, "C_BaseEntity->m_nSubclassID", 0x8); -- TODO: Do we need the VData? I'm pretty sure ammo is stored there...
};

class C_BaseModelEntity;

class CGlowProperty {
public:
	CS_CLASS_NO_INITIALIZER(CGlowProperty);

	SCHEMA_ADD_FIELD(Vector, m_fGlowColor, "CGlowProperty->m_fGlowColor");
	SCHEMA_ADD_FIELD(int, m_iGlowType, "CGlowProperty->m_iGlowType");
	SCHEMA_ADD_FIELD(int, m_iGlowTeam, "CGlowProperty->m_iGlowTeam");
	SCHEMA_ADD_FIELD(int, m_nGlowRange, "CGlowProperty->m_nGlowRange");
	SCHEMA_ADD_FIELD(int, m_nGlowRangeMin, "CGlowProperty->m_nGlowRangeMin");
	SCHEMA_ADD_FIELD(Color, m_glowColorOverride, "CGlowProperty->m_glowColorOverride");
	SCHEMA_ADD_FIELD(bool, m_bFlashing, "CGlowProperty->m_bFlashing");
	SCHEMA_ADD_FIELD(float, m_flGlowTime, "CGlowProperty->m_flGlowTime");
	SCHEMA_ADD_FIELD(float, m_flGlowStartTime, "CGlowProperty->m_flGlowStartTime");
	SCHEMA_ADD_FIELD(bool, m_bEligibleForScreenHighlight, "CGlowProperty->m_bEligibleForScreenHighlight");
	SCHEMA_ADD_FIELD(bool, m_bGlowing, "CGlowProperty->m_bGlowing");

	C_BaseEntity* Owner() {
		return *(C_BaseEntity**)(this + 0x18); // TODO: fix this abomination
	}
};

class C_BaseModelEntity : public C_BaseEntity
{
public:
	CS_CLASS_NO_INITIALIZER(C_BaseModelEntity);

	SCHEMA_ADD_FIELD(CCollisionProperty, m_Collision, "C_BaseModelEntity->m_Collision");
	SCHEMA_ADD_FIELD(CGlowProperty, m_Glow, "C_BaseModelEntity->m_Glow");
	SCHEMA_ADD_FIELD(Vector, m_vecViewOffset, "C_BaseModelEntity->m_vecViewOffset");
	SCHEMA_ADD_FIELD(CBaseHandle, m_hOldMoveParent, "C_BaseModelEntity->m_hOldMoveParent");
};

class CPlayer_ItemServices {
public:
	CS_CLASS_NO_INITIALIZER(CPlayer_ItemServices);

	SCHEMA_ADD_FIELD(bool, m_bHasDefuser, "CCSPlayer_ItemServices->m_bHasDefuser");
	SCHEMA_ADD_FIELD(bool, m_bHasHelmet, "CCSPlayer_ItemServices->m_bHasHelmet");
	SCHEMA_ADD_FIELD(bool, m_bHasHeavyArmor, "CCSPlayer_ItemServices->m_bHasHeavyArmor");
};
class CPlayer_CameraServices {
public:
	CS_CLASS_NO_INITIALIZER(CPlayer_CameraServices);

	SCHEMA_ADD_FIELD(QAngle, m_vecCsViewPunchAngle, "CPlayer_CameraServices->m_vecCsViewPunchAngle");
	SCHEMA_ADD_FIELD(GameTick_t, m_nCsViewPunchAngleTick, "CPlayer_CameraServices->m_nCsViewPunchAngleTick");
	SCHEMA_ADD_FIELD(float_t, m_flCsViewPunchAngleTickRatio, "CPlayer_CameraServices->m_flCsViewPunchAngleTickRatio");
	SCHEMA_ADD_FIELD(float_t, m_flOldPlayerZ, "CPlayer_CameraServices->m_flOldPlayerZ");
	SCHEMA_ADD_FIELD(float_t, m_flOldPlayerViewOffsetZ, "CPlayer_CameraServices->m_flOldPlayerViewOffsetZ");
	SCHEMA_ADD_FIELD(bool[5], m_bOverrideFogColor, "CPlayer_CameraServices->m_bOverrideFogColor");
	SCHEMA_ADD_FIELD(Color[5], m_OverrideFogColor, "CPlayer_CameraServices->m_OverrideFogColor");
	SCHEMA_ADD_FIELD(bool[5], m_bOverrideFogStartEnd, "CPlayer_CameraServices->m_bOverrideFogStartEnd");
	SCHEMA_ADD_FIELD(float_t[5], m_fOverrideFogStart, "CPlayer_CameraServices->m_fOverrideFogStart");
	SCHEMA_ADD_FIELD(float_t[5], m_fOverrideFogEnd, "CPlayer_CameraServices->m_fOverrideFogEnd");
	SCHEMA_ADD_FIELD(CBaseHandle, m_hActivePostProcessingVolume, "CPlayer_CameraServices->m_hActivePostProcessingVolume");
};

template <typename T>
class C_NetworkUtlVectorBase {
public:
	uint32_t nSize;
	T* pElements;

	bool Contains(T Element) {
		if (!pElements) // Stupid fucking array having stupid fucking nullptr elements
			return false;

		for (uint32_t i = 0; i < nSize; i++)
		{
			if (pElements[i] == Element)
				return true;
		}
		return false;
	}
};

class CPlayer_WeaponServices
{
public:
	SCHEMA_ADD_FIELD(CBaseHandle, m_hActiveWeapon, "CPlayer_WeaponServices->m_hActiveWeapon");
	SCHEMA_ADD_FIELD(C_NetworkUtlVectorBase<CBaseHandle>, m_hMyWeapons, "CPlayer_WeaponServices->m_hMyWeapons"); // CBasePlayerWeapon
};

class CCSPlayer_WeaponServices : public CPlayer_WeaponServices
{
public:
	SCHEMA_ADD_FIELD(GameTime_t, m_flNextAttack, "CCSPlayer_WeaponServices->m_flNextAttack");
};

enum class ObserverMode_t : uint8_t
{
	OBS_MODE_NONE = 0x0,
	OBS_MODE_FIXED = 0x1,
	OBS_MODE_IN_EYE = 0x2,
	OBS_MODE_CHASE = 0x3,
	OBS_MODE_ROAMING = 0x4,
	OBS_MODE_DIRECTED = 0x5,
	NUM_OBSERVER_MODES = 0x6,
};

class CPlayer_ObserverServices {
public:
	CS_CLASS_NO_INITIALIZER(CPlayer_ObserverServices);

	SCHEMA_ADD_FIELD(ObserverMode_t, m_iObserverMode, "CPlayer_ObserverServices->m_iObserverMode");
	SCHEMA_ADD_FIELD(CBaseHandle, m_hObserverTarget, "CPlayer_ObserverServices->m_hObserverTarget");
	SCHEMA_ADD_FIELD(ObserverMode_t, m_iObserverLastMode, "CPlayer_ObserverServices->m_iObserverLastMode");
	SCHEMA_ADD_FIELD(bool, m_bForcedObserverMode, "CPlayer_ObserverServices->m_bForcedObserverMode");
	SCHEMA_ADD_FIELD(float, m_flObserverChaseDistance, "CPlayer_ObserverServices->m_flObserverChaseDistance");
	SCHEMA_ADD_FIELD(GameTime_t, m_flObserverChaseDistanceCalcTime, "CPlayer_ObserverServices->m_flObserverChaseDistanceCalcTime");
};

class CCSPlayerController;
class C_BasePlayerPawn : public C_BaseModelEntity {
public:
	CS_CLASS_NO_INITIALIZER(C_BasePlayerPawn);

	SCHEMA_ADD_FIELD(CCSPlayer_WeaponServices*, m_pWeaponServices, "C_BasePlayerPawn->m_pWeaponServices");
	SCHEMA_ADD_FIELD(CPlayer_ItemServices*, m_pItemServices, "C_BasePlayerPawn->m_pItemServices");
	SCHEMA_ADD_FIELD(CPlayer_ObserverServices*, m_pObserverServices, "C_BasePlayerPawn->m_pObserverServices");
	SCHEMA_ADD_FIELD(CPlayer_CameraServices*, m_pCameraServices, "C_BasePlayerPawn->m_pCameraServices");
	SCHEMA_ADD_FIELD(CCSPlayer_MovementServices*, m_pMovementServices, "C_BasePlayerPawn->m_pMovementServices");
	SCHEMA_ADD_FIELD(uint32_t, m_nHighestConsumedServerViewAngleChangeIndex, "C_BasePlayerPawn->m_nHighestConsumedServerViewAngleChangeIndex");
	SCHEMA_ADD_FIELD(QAngle, v_angle, "C_BasePlayerPawn->v_angle");
	SCHEMA_ADD_FIELD(QAngle, v_anglePrevious, "C_BasePlayerPawn->v_anglePrevious");
	SCHEMA_ADD_FIELD(uint32_t, m_iHideHUD, "C_BasePlayerPawn->m_iHideHUD");
	SCHEMA_ADD_FIELD(GameTime_t, m_flDeathTime, "C_BasePlayerPawn->m_flDeathTime");
	SCHEMA_ADD_FIELD(Vector, m_vecPredictionError, "C_BasePlayerPawn->m_vecPredictionError");
	SCHEMA_ADD_FIELD(GameTime_t, m_flPredictionErrorTime, "C_BasePlayerPawn->m_flPredictionErrorTime");
	SCHEMA_ADD_FIELD(Vector, m_vecLastCameraSetupLocalOrigin, "C_BasePlayerPawn->m_vecLastCameraSetupLocalOrigin");
	SCHEMA_ADD_FIELD(GameTime_t, m_flLastCameraSetupTime, "C_BasePlayerPawn->m_flLastCameraSetupTime");
	SCHEMA_ADD_FIELD(float, m_flFOVSensitivityAdjust, "C_BasePlayerPawn->m_flFOVSensitivityAdjust");
	SCHEMA_ADD_FIELD(float, m_flMouseSensitivity, "C_BasePlayerPawn->m_flMouseSensitivity");
	SCHEMA_ADD_FIELD(Vector, m_vOldOrigin, "C_BasePlayerPawn->m_vOldOrigin");
	SCHEMA_ADD_FIELD(float, m_flOldSimulationTime, "C_BasePlayerPawn->m_flOldSimulationTime");
	SCHEMA_ADD_FIELD(int32_t, m_nLastExecutedCommandNumber, "C_BasePlayerPawn->m_nLastExecutedCommandNumber");
	SCHEMA_ADD_FIELD(int32_t, m_nLastExecutedCommandTick, "C_BasePlayerPawn->m_nLastExecutedCommandTick");
	SCHEMA_ADD_FIELD(CBaseHandle, m_hController, "C_BasePlayerPawn->m_hController");
	SCHEMA_ADD_FIELD(bool, m_bIsSwappingToPredictableController, "C_BasePlayerPawn->m_bIsSwappingToPredictableController");
};

class C_CSWeaponBase;
class C_BaseViewModel : public C_BaseModelEntity {
public:
	SCHEMA_ADD_FIELD(CBaseHandle, m_hWeapon, "C_BaseViewModel->m_hWeapon")
};

class C_CSGOViewModel : public C_BaseViewModel {
public:
};

class C_ViewmodelAttachmentModel : public C_BaseEntity {
public:
};

class CCSPlayer_ViewModelServices {
public:
	SCHEMA_ADD_FIELD(CBaseHandle, m_hViewModel, "CCSPlayer_ViewModelServices->m_hViewModel");
};

class C_CSPlayerPawnBase : public C_BasePlayerPawn
{
public:
	CS_CLASS_NO_INITIALIZER(C_CSPlayerPawnBase);

	SCHEMA_ADD_FIELD(CCSPlayer_ViewModelServices*, m_pViewModelServices, "C_CSPlayerPawnBase->m_pViewModelServices");
	SCHEMA_ADD_FIELD(float, m_flFlashMaxAlpha, "C_CSPlayerPawnBase->m_flFlashMaxAlpha");
	SCHEMA_ADD_FIELD(float, m_flFlashDuration, "C_CSPlayerPawnBase->m_flFlashDuration");
	SCHEMA_ADD_FIELD(Vector, m_vLastSmokeOverlayColor, "C_CSPlayerPawnBase->m_vLastSmokeOverlayColor");
	SCHEMA_ADD_FIELD(float, m_flLastSpawnTimeIndex, "C_CSPlayerPawnBase->m_flLastSpawnTimeIndex");
	SCHEMA_ADD_FIELD(bool, m_bGunGameImmunity, "C_CSPlayerPawnBase->m_bGunGameImmunity");
	SCHEMA_ADD_FIELD(QAngle, m_angEyeAngles, "C_CSPlayerPawnBase->m_angEyeAngles");
};

class C_CSWeaponBase;
class C_EconItemView;

class C_CSPlayerPawn : public C_CSPlayerPawnBase
{
public:
	CS_CLASS_NO_INITIALIZER(C_CSPlayerPawn);

	bool IsAlive() {
		return (this->m_lifeState() == 0U || this->m_iHealth() > 0);
	}

	SCHEMA_ADD_FIELD(QAngle, m_aimPunchAngle, "C_CSPlayerPawn->m_aimPunchAngle");
	SCHEMA_ADD_FIELD(bool, m_bIsScoped, "C_CSPlayerPawn->m_bIsScoped");
	SCHEMA_ADD_FIELD(bool, m_bIsDefusing, "C_CSPlayerPawn->m_bIsDefusing");
	SCHEMA_ADD_FIELD(bool, m_bInLanding, "C_CSPlayerPawn->m_bInLanding");
	SCHEMA_ADD_FIELD(float, m_flLandingTimeSeconds, "C_CSPlayerPawn->m_flLandingTimeSeconds");
	SCHEMA_ADD_FIELD(float, m_flLandingStartTime, "C_CSPlayerPawn->m_flLandingStartTime");
	SCHEMA_ADD_FIELD(bool, m_bResumeZoom, "C_CSPlayerPawn->m_bResumeZoom");
	SCHEMA_ADD_FIELD(bool, m_bIsGrabbingHostage, "C_CSPlayerPawn->m_bIsGrabbingHostage");
	SCHEMA_ADD_FIELD(int32_t, m_ArmorValue, "C_CSPlayerPawn->m_ArmorValue");
	SCHEMA_ADD_FIELD(bool, m_bWaitForNoAttack, "C_CSPlayerPawn->m_bWaitForNoAttack");
	SCHEMA_ADD_FIELD(bool, m_bNeedToReApplyGloves, "C_CSPlayerPawn->m_bNeedToReApplyGloves");
	SCHEMA_ADD_FIELD(float, m_flVelocityModifier, "C_CSPlayerPawn->m_flVelocityModifier");
	SCHEMA_ADD_FIELD(char[18], m_szLastPlaceName, "C_CSPlayerPawn->m_szLastPlaceName");

	SCHEMA_ADD_FIELD(int32_t, m_iShotsFired, "C_CSPlayerPawn->m_iShotsFired");

	std::uint16_t m_iCollisionMask() {
		if (m_pCollision())
			return m_pCollision()->CollisionMask();  // Collision + 0x38
		return 0;
	}
};

class CUserCmdManager;
class CBasePlayerController : public C_BaseModelEntity
{
public:
	CS_CLASS_NO_INITIALIZER(CBasePlayerController);

	SCHEMA_ADD_FIELD(std::uint64_t, m_steamID, "CBasePlayerController->m_steamID");
	SCHEMA_ADD_FIELD(std::uint32_t, m_nTickBase, "CBasePlayerController->m_nTickBase");
	SCHEMA_ADD_FIELD(CBaseHandle, m_hPawn, "CBasePlayerController->m_hPawn");
	SCHEMA_ADD_FIELD(bool, m_bIsLocalPlayerController, "CBasePlayerController->m_bIsLocalPlayerController");
};

// forward decleration
class C_CSWeaponBaseGun;
class C_BasePlayerWeapon;

// Both CCSPlayerController_InventoryServices and CCSPlayerController_InGameMoneyServices should derive from CPlayerControllerComponent, but eh, who cares
class CCSPlayerController_InventoryServices {
public:
	CS_CLASS_NO_INITIALIZER(CCSPlayerController_InventoryServices);

	SCHEMA_ADD_FIELD(uint16_t, m_unMusicID, "CCSPlayerController_InventoryServices->m_unMusicID");
};

class CCSPlayerController_InGameMoneyServices {
public:
	CS_CLASS_NO_INITIALIZER(CCSPlayerController_InGameMoneyServices);

	SCHEMA_ADD_FIELD(int32_t, m_iAccount, "CCSPlayerController_InGameMoneyServices->m_iAccount");
};

class CCSPlayerController : public CBasePlayerController
{
public:
	CS_CLASS_NO_INITIALIZER(CCSPlayerController);

	SCHEMA_ADD_FIELD(CCSPlayerController_InGameMoneyServices*, m_pInGameMoneyServices, "CCSPlayerController->m_pInGameMoneyServices");
	SCHEMA_ADD_FIELD(CCSPlayerController_InventoryServices*, m_pInventoryServices, "CCSPlayerController->m_pInventoryServices");
	SCHEMA_ADD_FIELD(std::uint32_t, m_iPing, "CCSPlayerController->m_iPing");
	SCHEMA_ADD_FIELD(const char*, m_sSanitizedPlayerName, "CCSPlayerController->m_sSanitizedPlayerName");
	SCHEMA_ADD_FIELD(std::int32_t, m_iPawnHealth, "CCSPlayerController->m_iPawnHealth");
	SCHEMA_ADD_FIELD(std::int32_t, m_iPawnArmor, "CCSPlayerController->m_iPawnArmor");
	SCHEMA_ADD_FIELD(uint8_t, m_nKillCount, "CCSPlayerController->m_nKillCount");
	SCHEMA_ADD_FIELD(bool, m_bPawnHasDefuser, "CCSPlayerController->m_bPawnHasDefuser");
	SCHEMA_ADD_FIELD(bool, m_bPawnHasHelmet, "CCSPlayerController->m_bPawnHasHelmet");
	SCHEMA_ADD_FIELD(bool, m_bPawnIsAlive, "CCSPlayerController->m_bPawnIsAlive");
	SCHEMA_ADD_FIELD(CBaseHandle, m_hPlayerPawn, "CCSPlayerController->m_hPlayerPawn");
};

class CBaseAnimGraph : public C_BaseModelEntity
{
public:
	CS_CLASS_NO_INITIALIZER(CBaseAnimGraph);

	SCHEMA_ADD_FIELD(bool, m_bAnimationUpdateScheduled, "CBaseAnimGraph->m_bAnimationUpdateScheduled");
};

class C_LocalTempEntity : public CBaseAnimGraph {
public:
	CS_CLASS_NO_INITIALIZER(C_LocalTempEntity);

	SCHEMA_ADD_FIELD(int32_t, flags, "C_LocalTempEntity->m_ragPos");
	SCHEMA_ADD_FIELD(GameTime_t, die, "C_LocalTempEntity->die");
	SCHEMA_ADD_FIELD(float, m_flFrameMax, "C_LocalTempEntity->m_flFrameMax");
	SCHEMA_ADD_FIELD(float, x, "C_LocalTempEntity->x");
	SCHEMA_ADD_FIELD(float, y, "C_LocalTempEntity->y");
	SCHEMA_ADD_FIELD(float, fadeSpeed, "C_LocalTempEntity->fadeSpeed");
	SCHEMA_ADD_FIELD(float, bounceFactor, "C_LocalTempEntity->bounceFactor");
	SCHEMA_ADD_FIELD(int32_t, hitSound, "C_LocalTempEntity->hitSound");
	SCHEMA_ADD_FIELD(int32_t, priority, "C_LocalTempEntity->priority");
	SCHEMA_ADD_FIELD(Vector, tentOffset, "C_LocalTempEntity->tentOffset");
	SCHEMA_ADD_FIELD(int32_t, tempent_renderamt, "C_LocalTempEntity->tempent_renderamt");
	SCHEMA_ADD_FIELD(Vector, m_vecNormal, "C_LocalTempEntity->m_vecNormal");
	SCHEMA_ADD_FIELD(float, m_flSpriteScale, "C_LocalTempEntity->m_flSpriteScale");
	SCHEMA_ADD_FIELD(int32_t, m_nFlickerFrame, "C_LocalTempEntity->m_nFlickerFrame");
	SCHEMA_ADD_FIELD(float, m_flFrameRate, "C_LocalTempEntity->m_flFrameRate");
	SCHEMA_ADD_FIELD(float, m_flFrame, "C_LocalTempEntity->m_flFrame");
	SCHEMA_ADD_FIELD(char*, m_pszImpactEffect, "C_LocalTempEntity->m_pszImpactEffect");
	SCHEMA_ADD_FIELD(char*, m_pszParticleEffect, "C_LocalTempEntity->m_pszParticleEffect");
	SCHEMA_ADD_FIELD(bool, m_bParticleCollision, "C_LocalTempEntity->m_bParticleCollision");
	SCHEMA_ADD_FIELD(int32_t, m_iLastCollisionFrame, "C_LocalTempEntity->m_iLastCollisionFrame");
	SCHEMA_ADD_FIELD(Vector, m_vLastCollisionOrigin, "C_LocalTempEntity->m_vLastCollisionOrigin");
	SCHEMA_ADD_FIELD(Vector, m_vecTempEntVelocity, "C_LocalTempEntity->m_vecTempEntVelocity");
	SCHEMA_ADD_FIELD(Vector, m_vecPrevAbsOrigin, "C_LocalTempEntity->m_vecPrevAbsOrigin");
	SCHEMA_ADD_FIELD(Vector, m_vecTempEntAcceleration, "C_LocalTempEntity->m_vecTempEntAcceleration");
};

class C_BaseFlex : public CBaseAnimGraph
{
public:
	CS_CLASS_NO_INITIALIZER(C_BaseFlex);
	// I don't care, I love it!
};

class CEconItemDefinition;
class CPaintKit;	
class C_EconItemView {
public:
	CS_CLASS_NO_INITIALIZER(C_EconItemView);

	auto GetCustomPaintKitIndex() { return MEM::CallVFunc<int, 2U>(this); }
	auto GetStaticData() {
		return MEM::CallVFunc<CEconItemDefinition*, 13>(this);
	}

	SCHEMA_ADD_FIELD(bool, m_bInventoryImageRgbaRequested, "C_EconItemView->m_bInventoryImageRgbaRequested");
	SCHEMA_ADD_FIELD(bool, m_bInventoryImageTriedCache, "C_EconItemView->m_bInventoryImageTriedCache");
	SCHEMA_ADD_FIELD(int, m_nInventoryImageRgbaWidth, "C_EconItemView->m_nInventoryImageRgbaWidth");
	SCHEMA_ADD_FIELD(int, m_nInventoryImageRgbaHeight, "C_EconItemView->m_nInventoryImageRgbaHeight");
	SCHEMA_ADD_FIELD(char[260], m_szCurrentLoadCachedFileName, "C_EconItemView->m_szCurrentLoadCachedFileName");
	SCHEMA_ADD_FIELD(bool, m_bRestoreCustomMaterialAfterPrecache, "C_EconItemView->m_bRestoreCustomMaterialAfterPrecache");
	SCHEMA_ADD_FIELD(unsigned short, m_iItemDefinitionIndex, "C_EconItemView->m_iItemDefinitionIndex");
	SCHEMA_ADD_FIELD(int, m_iEntityQuality, "C_EconItemView->m_iEntityQuality");
	SCHEMA_ADD_FIELD(unsigned int, m_iEntityLevel, "C_EconItemView->m_iEntityLevel");
	SCHEMA_ADD_FIELD(unsigned long, m_iItemID, "C_EconItemView->m_iItemID");
	SCHEMA_ADD_FIELD(unsigned int, m_iItemIDHigh, "C_EconItemView->m_iItemIDHigh");
	SCHEMA_ADD_FIELD(unsigned int, m_iItemIDLow, "C_EconItemView->m_iItemIDLow");
	SCHEMA_ADD_FIELD(unsigned int, m_iAccountID, "C_EconItemView->m_iAccountID");
	SCHEMA_ADD_FIELD(unsigned int, m_iInventoryPosition, "C_EconItemView->m_iInventoryPosition");
	SCHEMA_ADD_FIELD(bool, m_bInitialized, "C_EconItemView->m_bInitialized");
	SCHEMA_ADD_FIELD(bool, m_bDisallowSOC, "C_EconItemView->m_bDisallowSOC");
	SCHEMA_ADD_FIELD(bool, m_bIsStoreItem, "C_EconItemView->m_bIsStoreItem");
	SCHEMA_ADD_FIELD(bool, m_bIsTradeItem, "C_EconItemView->m_bIsTradeItem");
	SCHEMA_ADD_FIELD(int, m_iEntityQuantity, "C_EconItemView->m_iEntityQuantity");
	SCHEMA_ADD_FIELD(int, m_iRarityOverride, "C_EconItemView->m_iRarityOverride");
	SCHEMA_ADD_FIELD(int, m_iQualityOverride, "C_EconItemView->m_iQualityOverride");
	SCHEMA_ADD_FIELD(unsigned int, m_unClientFlags, "C_EconItemView->m_unClientFlags");
	SCHEMA_ADD_FIELD(unsigned int, m_unOverrideStyle, "C_EconItemView->m_unOverrideStyle");
	SCHEMA_ADD_FIELD(char[161], m_szCustomName, "C_EconItemView->m_szCustomName");
	SCHEMA_ADD_FIELD(char[161], m_szCustomNameOverride, "C_EconItemView->m_szCustomNameOverride");
	SCHEMA_ADD_FIELD(bool, m_bInitializedTags, "C_EconItemView->m_bInitializedTags");
};

class CAttributeManager
{
public:
	CS_CLASS_NO_INITIALIZER(CAttributeManager);
	virtual ~CAttributeManager() = 0;
};
static_assert(sizeof(CAttributeManager) == 0x8);

class C_AttributeContainer : public CAttributeManager
{
public:
	CS_CLASS_NO_INITIALIZER(C_AttributeContainer);

	[[nodiscard]] inline std::add_pointer_t<C_EconItemView> m_Item() {
		const static std::uint32_t uOffset = Schema::GetOffset(FNV1A::HashConst("C_AttributeContainer->m_Item")) + 0U;
		if (!this) return reinterpret_cast<std::add_pointer_t<C_EconItemView>>(0);
		return reinterpret_cast<std::add_pointer_t<C_EconItemView>>(reinterpret_cast<std::uint8_t*>(this) + (uOffset));
	};
};

class C_EconEntity : public C_BaseFlex
{
public:
	CS_CLASS_NO_INITIALIZER(C_EconEntity);

	[[nodiscard]] inline std::add_pointer_t<C_AttributeContainer> m_AttributeManager() {
		const static std::uint32_t uOffset = Schema::GetOffset(FNV1A::HashConst("C_EconEntity->m_AttributeManager")) + 0U;
		if (!this) return reinterpret_cast<std::add_pointer_t<C_AttributeContainer>>(0);
		return reinterpret_cast<std::add_pointer_t<C_AttributeContainer>>(reinterpret_cast<std::uint8_t*>(this) + (uOffset));
	};
	SCHEMA_ADD_FIELD(std::uint32_t, m_OriginalOwnerXuidLow, "C_EconEntity->m_OriginalOwnerXuidLow");
	SCHEMA_ADD_FIELD(std::uint32_t, m_OriginalOwnerXuidHigh, "C_EconEntity->m_OriginalOwnerXuidHigh");
	SCHEMA_ADD_FIELD(std::int32_t, m_nFallbackPaintKit, "C_EconEntity->m_nFallbackPaintKit");
	SCHEMA_ADD_FIELD(std::int32_t, m_nFallbackSeed, "C_EconEntity->m_nFallbackSeed");
	SCHEMA_ADD_FIELD(std::int32_t, m_flFallbackWear, "C_EconEntity->m_flFallbackWear");
	SCHEMA_ADD_FIELD(std::int32_t, m_nFallbackStatTrak, "C_EconEntity->m_nFallbackStatTrak");
	SCHEMA_ADD_FIELD(CBaseHandle, m_hViewmodelAttachment, "C_EconEntity->m_hViewmodelAttachment");

	uint64_t m_OriginalOwnerXuid() {
		return ((uint64_t)(m_OriginalOwnerXuidHigh()) << 32) |
			m_OriginalOwnerXuidLow();
	}
};

class C_EconWearable : public C_EconEntity
{
public:
	CS_CLASS_NO_INITIALIZER(C_EconWearable);

	SCHEMA_ADD_FIELD(std::int32_t, m_nForceSkin, "C_EconWearable->m_nForceSkin");
	SCHEMA_ADD_FIELD(bool, m_bAlwaysAllow, "C_EconWearable->m_bAlwaysAllow");
};

class C_BasePlayerWeapon : public C_EconEntity
{
public:
	CS_CLASS_NO_INITIALIZER(C_BasePlayerWeapon);

	SCHEMA_ADD_FIELD(GameTick_t, m_nNextPrimaryAttackTick, "C_BasePlayerWeapon->m_nNextPrimaryAttackTick");
	SCHEMA_ADD_FIELD(float, m_flNextPrimaryAttackTickRatio, "C_BasePlayerWeapon->m_flNextPrimaryAttackTickRatio");
	SCHEMA_ADD_FIELD(GameTick_t, m_nNextSecondaryAttackTick, "C_BasePlayerWeapon->m_nNextSecondaryAttackTick");
	SCHEMA_ADD_FIELD(float, m_flNextSecondaryAttackTickRatio, "C_BasePlayerWeapon->m_flNextSecondaryAttackTickRatio");
	SCHEMA_ADD_FIELD(std::int32_t, m_iClip1, "C_BasePlayerWeapon->m_iClip1");
	SCHEMA_ADD_FIELD(std::int32_t, m_iClip2, "C_BasePlayerWeapon->m_iClip2");
	SCHEMA_ADD_FIELD(std::int32_t[2], m_pReserveAmmo, "C_BasePlayerWeapon->m_pReserveAmmo");
};

#define PSCHEMA_FIELD_OFFSET(varName, datatable, propName, extra_offset, type) \
    auto varName() {                                                           \
        static auto datatable_hash = FNV1A::Hash(datatable); \
        static auto prop_hash = FNV1A::Hash(propName);       \
                                                                               \
        static const auto m_offset =                                           \
            schema::GetOffset(datatable, datatable_hash, propName, prop_hash); \
                                                                               \
        return reinterpret_cast<std::add_pointer_t<type>>(                     \
            (uintptr_t)(this) + m_offset + extra_offset);                      \
    }

/// <summary>
/// Gets the Weapon Name from the Definition Index
/// </summary>
std::string GetWeaponNameFromDefIndex(int nDefIndex);

/// <summary>
/// Gets the Weapon small name from the Definition Index
/// </summary>
std::string GetWeaponIdFromDefIndex(int nDefIndex);

class CBasePlayerVData
{
public:
	CS_CLASS_NO_INITIALIZER(CBasePlayerVData);
};

class CBasePlayerWeaponVData
{
public:
	CS_CLASS_NO_INITIALIZER(CBasePlayerWeaponVData);

	SCHEMA_ADD_FIELD(std::int32_t, m_iMaxClip1, "CBasePlayerWeaponVData->m_iMaxClip1");
};

using CFiringModeFloat = std::float_t[2];

class CCSWeaponBaseVData : public CBasePlayerWeaponVData
{
public:
	CS_CLASS_NO_INITIALIZER(CCSWeaponBaseVData);

	SCHEMA_ADD_OFFSET(const char*, m_szName, 0xC20);
	SCHEMA_ADD_FIELD(std::int32_t, m_WeaponType, "CCSWeaponBaseVData->m_WeaponType");
	SCHEMA_ADD_FIELD(bool, m_bIsFullAuto, "CCSWeaponBaseVData->m_bIsFullAuto");
	SCHEMA_ADD_FIELD(CFiringModeFloat, m_flCycleTime, "CCSWeaponBaseVData->m_flCycleTime");
	SCHEMA_ADD_FIELD(int, m_nNumBullets, "CCSWeaponBaseVData->m_nNumBullets");
	SCHEMA_ADD_FIELD(int, m_nDamage, "CCSWeaponBaseVData->m_nDamage");
	SCHEMA_ADD_FIELD(float, m_flPenetration, "CCSWeaponBaseVData->m_flPenetration");
	SCHEMA_ADD_FIELD(float, m_flRangeModifier, "CCSWeaponBaseVData->m_flRangeModifier");
	SCHEMA_ADD_FIELD(float, m_flRange, "CCSWeaponBaseVData->m_flRange");
	SCHEMA_ADD_FIELD(float, m_flHeadshotMultiplier, "CCSWeaponBaseVData->m_flHeadshotMultiplier");
	SCHEMA_ADD_FIELD(float, m_flArmorRatio, "CCSWeaponBaseVData->m_flArmorRatio");
	SCHEMA_ADD_FIELD(bool, m_bMeleeWeapon, "CCSWeaponBaseVData->m_bMeleeWeapon");
	SCHEMA_ADD_FIELD(bool, m_bHasBurstMode, "CCSWeaponBaseVData->m_bHasBurstMode");
	SCHEMA_ADD_FIELD(bool, m_bIsRevolver, "CCSWeaponBaseVData->m_bIsRevolver");

	bool IsPrimary()
	{
		switch (m_WeaponType())
		{
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			return true;
		default:
			return false;
		}
	}
};

class C_CSWeaponBase : public C_BasePlayerWeapon
{
public:
	CS_CLASS_NO_INITIALIZER(C_CSWeaponBase);

	SCHEMA_ADD_FIELD(bool, m_bInReload, "C_CSWeaponBase->m_bInReload");
	SCHEMA_ADD_FIELD(bool, m_bUIWeapon, "C_CSWeaponBase->m_bUIWeapon");
	SCHEMA_ADD_FIELD(GameTick_t, m_nPostponeFireReadyTicks, "C_CSWeaponBase->m_nPostponeFireReadyTicks");
	SCHEMA_ADD_FIELD_OFFSET(void*, m_pStattrakAttachment, "C_CSWeaponBase->m_iNumEmptyAttacks", 4);
	SCHEMA_ADD_FIELD_OFFSET(void*, m_pNametagAttachment, "C_CSWeaponBase->m_iNumEmptyAttacks", 20);
	SCHEMA_ADD_FIELD(int, m_iRecoilIndex, "C_CSWeaponBase->m_iRecoilIndex");
	SCHEMA_ADD_FIELD(float, m_flRecoilIndex, "C_CSWeaponBase->m_flRecoilIndex");
	SCHEMA_ADD_FIELD(float, m_fAccuracyPenalty, "C_CSWeaponBase->m_fAccuracyPenalty");
	SCHEMA_ADD_FIELD(int, m_iOriginalTeamNumber, "C_CSWeaponBase->m_iOriginalTeamNumber");

	CCSWeaponBaseVData* m_WpnVData()
	{
		return static_cast<CCSWeaponBaseVData*>(m_VData());
	}
};

class C_CSWeaponBaseGun : public C_CSWeaponBase
{
public:
	CS_CLASS_NO_INITIALIZER(C_CSWeaponBaseGun);

	SCHEMA_ADD_FIELD(std::int32_t, m_zoomLevel, "C_CSWeaponBaseGun->m_zoomLevel");
	SCHEMA_ADD_FIELD(std::int32_t, m_iBurstShotsRemaining, "C_CSWeaponBaseGun->m_iBurstShotsRemaining");
	SCHEMA_ADD_FIELD(bool, m_bBurstMode, "C_CSWeaponBase->m_bBurstMode");
	SCHEMA_ADD_FIELD(float, m_flPostponeFireReadyFrac, "C_CSWeaponBase->m_flPostponeFireReadyFrac");
};

class C_BaseCSGrenade : public C_CSWeaponBase
{
public:
	SCHEMA_ADD_FIELD(bool, IsHeldByPlayer, "C_BaseCSGrenade->m_bIsHeldByPlayer");
	SCHEMA_ADD_FIELD(bool, m_bPinPulled, "C_BaseCSGrenade->m_bPinPulled");
	SCHEMA_ADD_FIELD(GameTime_t, m_fThrowTime, "C_BaseCSGrenade->m_fThrowTime");
	SCHEMA_ADD_FIELD(float, m_flThrowStrength, "C_BaseCSGrenade->m_flThrowStrength");
};

class C_BaseGrenade : public C_BaseFlex
{
public:
	CS_CLASS_NO_INITIALIZER(C_BaseGrenade);

	SCHEMA_ADD_FIELD(float, m_DmgRadius, "C_BaseGrenade->m_DmgRadius");
	SCHEMA_ADD_FIELD(float, m_flDamage, "C_BaseGrenade->m_flDamage");
};

class C_BaseCSGrenadeProjectile : public C_BaseGrenade {
public:
	CS_CLASS_NO_INITIALIZER(C_BaseCSGrenadeProjectile);

	SCHEMA_ADD_FIELD(Vector, m_vInitialPosition, "C_BaseCSGrenadeProjectile->m_vInitialPosition");
	SCHEMA_ADD_FIELD(Vector, m_vInitialVelocity, "C_BaseCSGrenadeProjectile->m_vInitialVelocity");
	SCHEMA_ADD_FIELD(int, m_nBounces, "C_BaseCSGrenadeProjectile->m_nBounces");
	SCHEMA_ADD_FIELD(int, m_nExplodeEffectTickBegin, "C_BaseCSGrenadeProjectile->m_nExplodeEffectTickBegin");
	SCHEMA_ADD_FIELD(Vector, m_vecExplodeEffectOrigin, "C_BaseCSGrenadeProjectile->m_vecExplodeEffectOrigin");
	SCHEMA_ADD_FIELD(GameTime_t, m_flSpawnTime, "C_BaseCSGrenadeProjectile->m_flSpawnTime");
	SCHEMA_ADD_FIELD(Vector, vecLastTrailLinePos, "C_BaseCSGrenadeProjectile->vecLastTrailLinePos");
	SCHEMA_ADD_FIELD(GameTime_t, flNextTrailLineTime, "C_BaseCSGrenadeProjectile->flNextTrailLineTime");
	SCHEMA_ADD_FIELD(bool, m_bExplodeEffectBegan, "C_BaseCSGrenadeProjectile->m_bExplodeEffectBegan");
	SCHEMA_ADD_FIELD(bool, m_bCanCreateGrenadeTrail, "C_BaseCSGrenadeProjectile->m_bCanCreateGrenadeTrail");
	SCHEMA_ADD_FIELD(float, m_flTrajectoryTrailEffectCreationTime, "C_BaseCSGrenadeProjectile->m_flTrajectoryTrailEffectCreationTime");
};

class C_PlantedC4 : public CBaseAnimGraph {
public:
	CS_CLASS_NO_INITIALIZER(C_PlantedC4);

	SCHEMA_ADD_FIELD(bool, m_bBeingDefused, "C_PlantedC4->m_bBeingDefused");
	SCHEMA_ADD_FIELD(bool, m_bRadarFlash, "C_PlantedC4->m_bRadarFlash");
	SCHEMA_ADD_FIELD(CBaseHandle, m_pBombDefuser, "C_PlantedC4->m_pBombDefuser");
	SCHEMA_ADD_FIELD(int32_t, m_nBombSite, "C_PlantedC4->m_nBombSite");
	SCHEMA_ADD_FIELD(float, m_flTimerLength, "C_PlantedC4->m_flTimerLength");
	SCHEMA_ADD_FIELD(float, m_flDefuseLength, "C_PlantedC4->m_flDefuseLength");
	SCHEMA_ADD_FIELD(GameTime_t, m_flDefuseCountDown, "C_PlantedC4->m_flDefuseCountDown");
	SCHEMA_ADD_FIELD(float, m_bHasExploded, "C_PlantedC4->m_bHasExploded");
	SCHEMA_ADD_FIELD(GameTime_t, m_flC4Blow, "C_PlantedC4->m_flC4Blow");
};

class C_C4 : public C_CSWeaponBase {
public:
	CS_CLASS_NO_INITIALIZER(C_C4);

	SCHEMA_ADD_FIELD(bool, m_bStartedArming, "C_C4->m_bStartedArming");
	SCHEMA_ADD_FIELD(float, m_fArmedTime, "C_C4->m_fArmedTime");
};

#define STRINGTOKEN_MURMURHASH_SEED 0x31415926

inline uint32_t QAMurmurHash2(const void* key, int len, uint32_t seed)
{
	const uint32_t m = 0x5bd1e995;
	const int r = 24;

	uint32_t h = seed ^ len;
	const unsigned char* data = (const unsigned char*)key;

	while (len >= 4)
	{
		uint32_t k = *(uint32_t*)data;

		k *= m;
		k ^= k >> r;
		k *= m;

		h *= m;
		h ^= k;

		data += 4;
		len -= 4;
	}

	switch (len)
	{
	case 3: h ^= data[2] << 16;
	case 2: h ^= data[1] << 8;
	case 1: h ^= data[0];
		h *= m;
	};

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
}

struct EventHash_t {
	EventHash_t(const char* String)
	{
		mString = String;
		mHashCode = QAMurmurHash2(String, std::strlen(String), STRINGTOKEN_MURMURHASH_SEED);
	}

	std::uint64_t mHashCode;
	const char* mString;
};

class IGameEvent {
public:
	const char* GetName() {
		return MEM::CallVFunc<const char*, 1>(this);
	}

	CBasePlayerController* GetPlayerController(const char* keyName) {
		// I don't know the real name of this function. It calls some function
		// with the value returned by GetInt().

		EventHash_t arg(keyName);
		return MEM::CallVFunc<CBasePlayerController*, 16U>(this, &arg);
	}

	bool GetBool(const char* keyName) {
		EventHash_t arg(keyName);
		return MEM::CallVFunc<bool, 6>(this, &arg, false);
	}

	int GetInt(const char* keyName) {
		EventHash_t arg(keyName);
		return MEM::CallVFunc<int, 7>(this, &arg, 0);
	}

	float GetFloat(const char* keyName) {
		EventHash_t arg(keyName);
		return MEM::CallVFunc<float, 9>(this, &arg, 0.0f);
	}

	const char* GetString(const char* keyName) {
		EventHash_t arg(keyName);
		return MEM::CallVFunc<const char*, 10U>(this, &arg, nullptr);
	}
};

class C_Team : public C_BaseEntity {
public:
	SCHEMA_ADD_FIELD(C_NetworkUtlVectorBase<CBaseHandle>, m_aPlayerControllers, "C_Team->m_aPlayerControllers");
	SCHEMA_ADD_FIELD(C_NetworkUtlVectorBase<CBaseHandle>, m_aPlayers, "C_Team->m_aPlayers");
	SCHEMA_ADD_FIELD(int, m_iScore, "C_Team->m_iScore");
	SCHEMA_ADD_FIELD(char[129], m_szTeamname, "C_Team->m_szTeamname");
};