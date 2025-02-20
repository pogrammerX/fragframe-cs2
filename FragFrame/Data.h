// Purpose: store all of the necessary data we need

#pragma once

class CGameEntitySystem;
class C_CSGameRules;
class ISchemaSystem;
class CBaseFileSystem;
class CBaseHandle;
class IDXGISwapChain;
class C_CSPlayerPawn;
class CPanoramaUIEngine;
class CCSPlayerController;
class IInputSystem;
class IEngineCVar;
class IEngineClient;
class ID3D11Device;
class ID3D11DeviceContext;
class ID3D11RenderTargetView;
class IGlobalVars;
class C_Team;
struct ImFont;

namespace CSData {
	// extern for thread-safety... i hate this...
	extern std::vector<C_CSPlayerPawn*> g_arrPlayers; // All of the connected players (AKA pawns), we should probably use the Controllers instead, but i dont care lol
	extern CGameEntitySystem* g_pGameEntitySystem;
	extern C_CSGameRules* g_pGameRules;
	extern CCSPlayerController* g_pLocalController;
	extern C_CSPlayerPawn* g_pCurrentSpectatingPlayer;
	extern bool g_bSpectatorThirdPerson;
	extern CBaseHandle* g_pTargetSpectatorOverride;
	extern CBaseFileSystem* g_pFileSystem;
	extern IEngineCVar* g_pEngineCVar;
	extern CPanoramaUIEngine* g_pPanoramaEngine;
	extern IGlobalVars* g_pGlobalVars;
	extern ISchemaSystem* g_pSchemaSystem;
	extern IEngineClient* g_pEngineClient;
	extern IInputSystem* g_pInputSystem;
	extern IDXGISwapChain* g_pSwapChain;
	extern ID3D11Device* g_pDevice;
	extern ID3D11DeviceContext* g_pContext;
	extern ID3D11RenderTargetView* g_pRenderTarget;

	extern bool g_bShowStartPopup;

	extern C_Team* g_pTTeam;
	extern C_Team* g_pCTTeam;
	extern std::vector<C_Team*> g_arrTeams;

	extern HWND g_Window;
}

namespace CSIData {
	extern int m_nHudTeamCounterLabel;
}

namespace Fonts {
	extern ImFont* g_pDefaultSmall;
	extern ImFont* g_pDefaultSmallBold;
	extern ImFont* g_pDefaultSmallItalic;
	extern ImFont* g_pDefaultLarge;
	extern ImFont* g_pDefaultLargeItalic;
	extern ImFont* g_pFontAwesome;
}