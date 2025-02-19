#pragma once

/// <summary>
/// Represents a hooked function.
/// -- NOTE: This is what is semi-risky here, if we can't find a way to fix this we just have to make every server -insecure ^^'
/// </summary>
class CHook {
	const char* m_szName; // Name of the Hook
	void* m_pOriginal; // Original function
	bool m_bOk; // Indicator if the Hook was created succesfully or not

public:
	CHook(const char* szName, void* pFunction, void* pTrampoline);
	bool IsOk();
	void* GetOriginal();
	const char* GetName();
};

class IDXGISwapChain;

class IGameEvent;

namespace HookFuncs {
	// Present - Used for rendering
	HRESULT Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	// LevelInit - Used for regetting pointers
	__int64* __fastcall LevelInit(void* pClientModeShared, const char* szNewMap);
	// LevelShutdown - Used for clearing data
	__int64 __fastcall LevelShutdown(void* pClientModeShared);
	// UpdateTimerLabel - Used to get round time / freeze time
	__int64 __fastcall UpdateTimerLabel(__int64 pHudTeamCounter);
	// OnAddEntity - Used to add entities to the entity list
	void __fastcall OnAddEntity(void* pThis, C_BaseEntity* pEntity, CBaseHandle hHandle);
	// OnAddEntity - Used to remove entities from the entity list
	void __fastcall OnRemoveEntity(void* pThis, C_BaseEntity* pEntity, CBaseHandle hHandle);
	// FireClientEvent - Used to capture Events like round_officialy_ended, player_death, player_hurt, etc...
	char __fastcall FireClientEvent(void* pThis, IGameEvent* pEvent);
	// FrameStageNotify - Used to update Player's data
	void __fastcall FrameStageNotify(void* pThis, int nStage);
	// MouseInputEnabled - Used to enable mouse input if Theme::bAllowMouse is enabled
	bool __fastcall MouseInputEnabled(void* pThis);
	// WindowProc - Used to intercept Windows messages to the CS2 window
	LRESULT WindowProc(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam);
}

// I hate thread safety, if someone knows another way to do this, PLEASE MAKE A PULL REQUEST OR ISSUE!!!
struct __hooks {
	CHook Present;
	CHook LevelInit;
	CHook UpdateTimerLabel;
	CHook LevelShutdown;
	CHook OnAddEntity;
	CHook OnRemoveEntity;
	CHook FireClientEvent;
	CHook FrameStageNotify;
	CHook MouseInputEnabled;
	WNDPROC WindowProcOrg;
};
extern __hooks* g_pHooks;