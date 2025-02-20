/*

	FragFrame - Customizable Counter-Strike 2 Overlay for Tournaments and LAN Events

	Developed by pogrammerX (https://github.com/pogrammerX)

*/

#include "inc.h"
#include <iomanip>
#include <filesystem>
#include "3rdparty/stbi.h"

namespace CSData {
    std::vector<C_CSPlayerPawn*> g_arrPlayers;
    CGameEntitySystem* g_pGameEntitySystem;
    C_CSGameRules* g_pGameRules;
    CCSPlayerController* g_pLocalController;
    C_CSPlayerPawn* g_pCurrentSpectatingPlayer;
    bool g_bSpectatorThirdPerson;
    CPanoramaUIEngine* g_pPanoramaEngine;
    CBaseHandle* g_pTargetSpectatorOverride;
    CBaseFileSystem* g_pFileSystem;
    IEngineCVar* g_pEngineCVar;
    IGlobalVars* g_pGlobalVars;
    ISchemaSystem* g_pSchemaSystem;
    IEngineClient* g_pEngineClient;
    IInputSystem* g_pInputSystem;
    IDXGISwapChain* g_pSwapChain;
    ID3D11Device* g_pDevice;
    ID3D11DeviceContext* g_pContext;
    ID3D11RenderTargetView* g_pRenderTarget;

    bool g_bShowStartPopup;

    C_Team* g_pTTeam;
    C_Team* g_pCTTeam;
    std::vector<C_Team*> g_arrTeams;

    HWND g_Window;
}

namespace CSIData {
    int m_nHudTeamCounterLabel;
}

namespace Fonts {
    ImFont* g_pDefaultSmall;
    ImFont* g_pDefaultSmallBold;
    ImFont* g_pDefaultSmallItalic;
    ImFont* g_pDefaultLarge;
    ImFont* g_pDefaultLargeItalic;
    ImFont* g_pFontAwesome;
}

namespace FFState {
    int g_nStage;
    CAnimation g_StartAnimation;
}
std::unordered_map<FNV1A_t, std::vector<ID3D11ShaderResourceView*>> g_mImagesForMaps;
std::unordered_map<FNV1A_t, IconData_t> g_mWeaponIcons;
std::vector<PlayerData*> g_arrDataPlayers;
float flCardScaling = 1.f;

__hooks* g_pHooks;

template< typename T >
std::string int_to_hex(T i)
{
    std::stringstream stream;
    stream << "0x"
        << std::setfill('0') << std::setw(sizeof(T) * 2)
        << std::hex << i;
    return stream.str();
}

// Probably THE WORST way to do this, but i really dont care
DWORD DataThread(LPVOID) {
    CCSPlayerController** ppLocalController = (CCSPlayerController**)MEM::GetAbsoluteAddress(MEM::PatternScan(MEM::GetModuleBaseHandle("client.dll"), "48 8B 0D ? ? ? ? 48 85 C9 74 ? 83 FF ?"), 0x3);
    while (true) {
        // I don't wanna talk about it
        __try { CSData::g_pLocalController = *ppLocalController; }
        __except (EXCEPTION_EXECUTE_HANDLER) { CSData::g_pLocalController = nullptr; }
        Sleep(5);
    }
    return 0;
}

DWORD EntryThread(LPVOID) {
    Sleep(200);
    std::string szDebugString = "FragFrame ";
    szDebugString += FF_VERSION;
    szDebugString += " found CS data (GameEntitySystem: ";
    szDebugString += int_to_hex((uintptr_t)CSData::g_pGameEntitySystem);
    szDebugString += ", ISchemaSystem: ";
    szDebugString += int_to_hex((uintptr_t)CSData::g_pSchemaSystem);
    szDebugString += ")";
    MEM::Msg(MEM::AcquireChannel("FragFrame"), szDebugString.c_str(), CS_CLR_BLUE);

    if (!CSData::g_pGameEntitySystem || !CSData::g_pSchemaSystem)
        MEM::Msg(MEM::AcquireChannel("FragFrame"), "Failed to find one or more Interfaces, are you using the latest version?", CS_CLR_RED);

    ISwapChainDx11* sw = **reinterpret_cast<ISwapChainDx11***>(MEM::GetAbsoluteAddress(MEM::PatternScan(MEM::GetModuleBaseHandle("rendersystemdx11.dll"), "66 0F 7F 0D ? ? ? ? 48 8B F7 66 0F 7F 05"), 0x4));
    CSData::g_pSwapChain = sw->pDXGISwapChain;

    if (!CSData::g_pSwapChain)
        MEM::Msg(MEM::AcquireChannel("FragFrame"), "CRITICAL! SwapChain not found!!! Are you using the latest version?", CS_CLR_RED);

    CSData::g_pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&CSData::g_pDevice);
    if (!CSData::g_pDevice)
        MEM::Msg(MEM::AcquireChannel("FragFrame"), "CRITICAL! Device is nullptr!!! Are you using the latest version?", CS_CLR_RED);

    CSData::g_pDevice->GetImmediateContext(&CSData::g_pContext);
    if (!CSData::g_pContext)
        MEM::Msg(MEM::AcquireChannel("FragFrame"), "CRITICAL! Context is nullptr!!! Are you using the latest version?", CS_CLR_RED);

    for (const auto& dirEntry : std::filesystem::directory_iterator(FF_PATH"\\weapons")) {
        std::string szExtension = dirEntry.path().extension().string();
        if (szExtension.find("png") == std::string::npos)
            continue; // Who put some random ass file in our weapons folder???

        std::string szFilename = dirEntry.path().filename().string();

        std::string szWeaponId = szFilename.substr(0, szFilename.find_first_of('.'));

        FNV1A_t uWeaponIdHash = FNV1A::Hash(szWeaponId.c_str());

        int w, h;
        int comp;
        const char* path = dirEntry.path().string().c_str();
        stbi_uc* data = stbi_load(path, &w, &h, &comp, STBI_rgb_alpha);
        int size = w * h * 4;
        ID3D11ShaderResourceView* srv = CreateTexture(std::vector<uint8_t>{data, data + size}, w, h);
        stbi_image_free(data);

        if (!srv)
            continue;

        IconData_t icon{};
        icon.m_pTex = srv;
        icon.m_uWidth = w;
        icon.m_uHeight = h;

        MEM::Msg(MEM::AcquireChannel("FragFrame"), ("Loaded weapon icon for " + szWeaponId).c_str(), CS_CLR_GREEN);
        g_mWeaponIcons.insert({ uWeaponIdHash, icon });
    }

    for (const auto& dirEntry : std::filesystem::directory_iterator(FF_PATH"\\radar")) {
        std::string szExtension = dirEntry.path().extension().string();
        if (szExtension.find("png") == std::string::npos)
            continue; // Who put some random ass file in our radar folder???

        std::string szFilename = dirEntry.path().filename().string();

        std::string szIdentifier = szFilename.substr(0, szFilename.find("_radar"));

        FNV1A_t uIdHash = FNV1A::Hash(szIdentifier.c_str());

        int w, h;
        int comp;
        const char* path = dirEntry.path().string().c_str();
        stbi_uc* data = stbi_load(path, &w, &h, &comp, STBI_rgb_alpha);
        int size = w * h * 4;
        ID3D11ShaderResourceView* srv = CreateTexture(std::vector<uint8_t>{data, data + size}, w, h);
        stbi_image_free(data);

        if (!srv)
            continue;

        MEM::Msg(MEM::AcquireChannel("FragFrame"), ("Loaded radar image for " + szIdentifier).c_str(), CS_CLR_GREEN);
        g_mRadarImages.insert({ uIdHash, srv });
    }

    for (const auto& dirEntry : std::filesystem::directory_iterator(FF_PATH"\\maps")) {
        std::string szExtension = dirEntry.path().extension().string();
        if (szExtension.find("png") == std::string::npos)
            continue; // Who put some random ass file in our maps folder???

        std::string szFilename = dirEntry.path().filename().string();

        size_t nMapIdLocation = szFilename.substr(3).find_first_of('_') + 3;

        std::string szAssociatedMap = szFilename.substr(0, nMapIdLocation);

        FNV1A_t uMapHash = FNV1A::Hash(szAssociatedMap.c_str());

        if (!g_mImagesForMaps.contains(uMapHash))
            g_mImagesForMaps.insert({ uMapHash, std::vector<ID3D11ShaderResourceView*>() });

        int w, h;
        int comp;
        const char* path = dirEntry.path().string().c_str();
        stbi_uc* data = stbi_load(path, &w, &h, &comp, STBI_rgb_alpha);
        int size = w * h * 4;
        ID3D11ShaderResourceView* srv = CreateTexture(std::vector<uint8_t>{data, data + size}, w, h);
        stbi_image_free(data);

        if (!srv)
            continue;

        MEM::Msg(MEM::AcquireChannel("FragFrame"), ("Loaded image \"" + szFilename + "\"").c_str(), CS_CLR_GREEN);
        g_mImagesForMaps[uMapHash].push_back(srv);
    }

    if(!Schema::Setup("client.dll"))
        MEM::Msg(MEM::AcquireChannel("FragFrame"), "CRITICAL! Failed to initialize Schema System!", CS_CLR_RED);
    else 
        MEM::Msg(MEM::AcquireChannel("FragFrame"), "Schema System initialized", CS_CLR_WHITE);

    g_pHooks->Present = CHook{ "Present", MEM::GetVFunc(CSData::g_pSwapChain, 8U), &HookFuncs::Present };
    g_pHooks->LevelInit = CHook{ "LevelInit", MEM::PatternScan(MEM::GetModuleBaseHandle("client.dll"), "48 89 5C 24 ? 56 48 83 EC ? 48 8B 0D ? ? ? ? 48 8B F2"), &HookFuncs::LevelInit };
    g_pHooks->LevelShutdown = CHook{ "LevelShutdown", MEM::PatternScan(MEM::GetModuleBaseHandle("client.dll"), "48 83 EC ? 48 8B 0D ? ? ? ? 48 8D 15 ? ? ? ? 45 33 C9 45 33 C0 48 8B 01 FF 50 30 48 85 C0 74 ? 48 8B 0D ? ? ? ? 48 8B D0 4C 8B 01 41 FF 50 40 48 83 C4 ?"), &HookFuncs::LevelShutdown };
    g_pHooks->UpdateTimerLabel = CHook{ "UpdateTimerLabel", MEM::PatternScan(MEM::GetModuleBaseHandle("client.dll"), "48 8B C4 55 56 41 54 41 57 48 83 EC ? 48 8B 2D ? ? ? ?"), &HookFuncs::UpdateTimerLabel };
    g_pHooks->OnAddEntity = CHook{ "OnAddEntity", MEM::PatternScan(MEM::GetModuleBaseHandle("client.dll"), "48 89 74 24 10 57 48 83 EC ? 48 8B F9 41 8B C0 B9 ? ? ? ?"), &HookFuncs::OnAddEntity };
    g_pHooks->OnRemoveEntity = CHook{ "OnRemoveEntity", MEM::PatternScan(MEM::GetModuleBaseHandle("client.dll"), "48 89 74 24 10 57 48 83 EC ? 48 8B F9 41 8B C0 25 ? ? ? ?"), &HookFuncs::OnRemoveEntity };
    g_pHooks->FireClientEvent = CHook{ "FireClientEvent", MEM::PatternScan(MEM::GetModuleBaseHandle("client.dll"), "48 89 5C 24 ? 56 57 41 54 48 83 EC 30 48 8B F2"), &HookFuncs::FireClientEvent };
    g_pHooks->FrameStageNotify = CHook{ "FrameStageNotify", MEM::PatternScan(MEM::GetModuleBaseHandle("client.dll"), "48 89 5C 24 10 56 48 83 EC ? 8B 05 ? ? ? ? 8D 5A FF"), &HookFuncs::FrameStageNotify };
    g_pHooks->MouseInputEnabled = CHook{ "MouseInputEnabled", MEM::PatternScan(MEM::GetModuleBaseHandle("client.dll"), "40 53 48 83 EC ? 80 B9 14 06 00 00 ?"), &HookFuncs::MouseInputEnabled };
    g_pHooks->WindowProcOrg = (WNDPROC)SetWindowLongPtrA(CSData::g_Window, GWLP_WNDPROC, (LONG_PTR)&HookFuncs::WindowProc);
    if(g_pHooks->WindowProcOrg) 
        MEM::Msg(MEM::AcquireChannel("FragFrame"), ("Hooked \"WindowProc\" successfully. (Function Pointer: " + int_to_hex((uintptr_t)g_pHooks->WindowProcOrg) + ")").c_str(), CS_CLR_GREEN);
    else 
        MEM::Msg(MEM::AcquireChannel("FragFrame"), "Failed to hook \"WindowProc\"!", CS_CLR_RED);
    MEM::Msg(MEM::AcquireChannel("FragFrame"), "FragFrame ready, waiting for you to start spectating.", CS_CLR_GREEN);
    CSData::g_bShowStartPopup = true;

    return 0;
}

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,
    DWORD fdwReason,
    LPVOID lpvReserved) 
{
    if (fdwReason == DLL_PROCESS_ATTACH) {
        // Let's start by initializing all of our CS vars
        CSData::g_arrPlayers = std::vector<C_CSPlayerPawn*>{};
        CSData::g_pGameEntitySystem = *(CGameEntitySystem**)MEM::GetAbsoluteAddress(MEM::PatternScan(MEM::GetModuleBaseHandle("client.dll"), "48 8B 0D ? ? ? ? 0F 57 C0 C7 44 24 30 ? ? ? ?"), 0x3); // dwGameEntitySystem
        CSData::g_pSchemaSystem = (ISchemaSystem*)MEM::GetAbsoluteAddress(MEM::PatternScan(MEM::GetModuleBaseHandle("schemasystem.dll"), "48 8B 0D ? ? ? ? 48 8B 55 80 4C 8B 81 B0 00 00 00 E9 ? ? ? ? 8B 53 60"), 0x3); // SchemaSystem_001
        CSData::g_pEngineClient = (IEngineClient*)MEM::GetAbsoluteAddress(MEM::PatternScan(MEM::GetModuleBaseHandle("engine2.dll"), "48 8B 05 ? ? ? ? 48 89 5D 07"), 0x3); // Source2EngineToClient001
        CSData::g_pGameRules = *(C_CSGameRules**)MEM::GetAbsoluteAddress(MEM::PatternScan(MEM::GetModuleBaseHandle("client.dll"), "48 8B 1D ? ? ? ? 80 7B 41 00"), 0x3); // dwGameRules
        CSData::g_pGlobalVars = *(IGlobalVars**)MEM::GetAbsoluteAddress(MEM::PatternScan(MEM::GetModuleBaseHandle("client.dll"), "48 89 15 ? ? ? ? 48 89 42"), 0x3); // dwGlobalVars
        CSData::g_pFileSystem = *(CBaseFileSystem**)MEM::GetAbsoluteAddress(MEM::PatternScan(MEM::GetModuleBaseHandle("client.dll"), "48 8B 0D ? ? ? ? 4C 8D 05 ? ? ? ? 48 8D 95 F0 00 00 00 48 8B 01 FF 90 80 01 00 00"), 0x3); // VFileSystem017
        CSData::g_pInputSystem = (IInputSystem*)MEM::GetAbsoluteAddress(MEM::PatternScan(MEM::GetModuleBaseHandle("inputsystem.dll"), "48 8B 05 ? ? ? ? 41 B0 ?"), 0x3); // InputSystemVersion001
        // TODO: Sig this (VEngineCvar007) https://github.com/a2x/cs2-dumper/blob/main/output/interfaces.hpp#L213
        CSData::g_pEngineCVar = (IEngineCVar*)((uintptr_t)MEM::GetModuleBaseHandle("tier0.dll") + 0x38E4B0); 

        // Hard to find!
        // X-Ref: "animdebugger" (CUtlBuffer::PutString((CUtlBuffer *)&v14, "animdebugger"); -> if statement)
        // X-Ref: "runcommand:%04d,tick:%u" (sub_675950(&v35, 0i64, "runcommand:%04d,tick:%u", *(unsigned int *)(a2 + 8), v6); -> *(void (__fastcall **)(__int64, __int64, __int64, __int64, double *))(*(_QWORD *)qword_1BCE468 + 128i64))()
        CSData::g_pPanoramaEngine = *(CPanoramaUIEngine**)MEM::GetAbsoluteAddress(MEM::PatternScan(MEM::GetModuleBaseHandle("client.dll"), "48 8B 0D ? ? ? ? 48 85 C9 74 ? 44 8B 4C 24 50"), 0x3, 0x10); // Why is this -0x10???

        CSData::g_pLocalController = nullptr;
        CSData::g_pTTeam = nullptr;
        CSData::g_pCTTeam = nullptr;
        CSIData::m_nHudTeamCounterLabel = INT_MAX;

        g_pHooks = (__hooks*)malloc(sizeof(__hooks)); // yikes
        memset(g_pHooks, 0, sizeof(__hooks));

        if (!CSData::g_pEngineClient) {
            // bruh
            return TRUE;
        }

        if (CSData::g_pEngineClient->IsInGame())
            CSData::g_pEngineClient->ClientCmd_Unrestricted("disconnect");

        CSData::g_pEngineClient->ClientCmd_Unrestricted("toggleconsole");

        CreateThread(NULL, NULL, EntryThread, NULL, NULL, NULL);
        CreateThread(NULL, NULL, DataThread, NULL, NULL, NULL);
    }
    return TRUE;
}