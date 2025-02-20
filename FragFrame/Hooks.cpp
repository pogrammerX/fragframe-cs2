#include "inc.h"
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include "3rdparty/stbi.h"
#include "3rdparty/minhook/MinHook.h"
#include "3rdparty/imgui/imgui.h"
#include "3rdparty/imgui/imgui_impl_dx11.h"
#include "3rdparty/imgui/imgui_impl_win32.h"
#include "3rdparty/imgui/imgui_internal.h" // Why is ImRect only in here???
#include <filesystem>

#pragma comment(lib, "winmm.lib")

// We use MinHook as the hooking backend.

static bool g_pBackendInitialized = false; // Keep track if we already initialized MinHook

CHook::CHook(const char* szName, void* pFunction, void* pTrampoline) {
	// Initialize MinHook if we need to
	if (!g_pBackendInitialized) {
		g_pBackendInitialized = true;
		MH_Initialize();
		MEM::Msg(MEM::AcquireChannel("FragFrame"), "Hooking Backend initialized", Color(1.f, 1.f, 1.f));
	}

	// Initialize all variables
	m_szName = szName;
	m_pOriginal = nullptr;
	m_bOk = false;

	// Make sure no null values are passed, nullptr in either one of these can indicate an outdated version of FragFrame
	if (!pFunction || !pTrampoline) {
		MEM::Msg(MEM::AcquireChannel("FragFrame"), ("CRITICAL! null values were passed to CHook constructor at \"" + std::string(szName) + "\"!!! Are you on the latest version?").c_str(), Color(1.f, 0.f, 0.f));
		return;
	}

	// Create the Hook
	MH_STATUS Status =
		MH_CreateHook(pFunction, pTrampoline, (LPVOID*)&m_pOriginal);

	if (Status != MH_OK) {
		// Failed to create the hook, for god knows why
		MEM::Msg(MEM::AcquireChannel("FragFrame"), ("Failed to create hook \"" + std::string(szName) + "\"! (Status: " + std::to_string(Status) + ")").c_str(), Color(1.f, 0.f, 0.f));
		return;
	}

	Status = MH_EnableHook(pFunction);

	if (Status != MH_OK) {
		// Failed to create the hook, for god knows why
		MEM::Msg(MEM::AcquireChannel("FragFrame"), ("Failed to enable hook \"" + std::string(szName) + "\"! (Status: " + std::to_string(Status) + ")").c_str(), Color(1.f, 0.f, 0.f));
		return;
	}
	MEM::Msg(MEM::AcquireChannel("FragFrame"), ("Hooked \"" + std::string(szName) + "\" successfully. (Function Pointer: " + int_to_hex((uintptr_t)pFunction) + ")").c_str(), Color(0.f, 1.f, 0.f));
	m_bOk = true;
}

bool CHook::IsOk() {
	return m_bOk;
}

void* CHook::GetOriginal() {
	return m_pOriginal;
}

const char* CHook::GetName() {
	return m_szName;
}

void HandleGameEvent(IGameEvent* pEvent) {
    if (!pEvent) // Can this ever happen?
        return;

    const char* szEventName = pEvent->GetName();
    FNV1A_t uEventHash = FNV1A::Hash(szEventName);
    if (uEventHash == FNV1A::HashConst("player_death")) {
        CBasePlayerController* pPlayer = pEvent->GetPlayerController("userid");
        CBasePlayerController* pAttacker = pEvent->GetPlayerController("attacker");

        if (!pPlayer)
            return;

        PlayerData* pData = GetPlayerDataFromController(pPlayer);

        if (!pData)
            return;

        pData->m_iDeaths += 1;

        if (pAttacker) {
            PlayerData* pAttackerData = GetPlayerDataFromController(pAttacker);

            if (!pAttackerData)
                return;

            pAttackerData->m_iKills += 1;
        }
    }
    else if (uEventHash == FNV1A::HashConst("player_hurt")) {
        // We synchronize player health in FrameStageNotify, because I said so
        CBasePlayerController* pPlayer = pEvent->GetPlayerController("userid");
        CBasePlayerController* pAttacker = pEvent->GetPlayerController("attacker");

        if (!pAttacker)
            return;

        if (!pPlayer)
            return;

        // We don't want to add it to DTR counter if it was team damage
        if (pAttacker->m_iTeamNum() == pPlayer->m_iTeamNum())
            return;

        PlayerData* pData = GetPlayerDataFromController(pAttacker);

        if (!pData)
            return;

        int nDamage = pEvent->GetInt("dmg_health");

        pData->m_nDamageThisRound += nDamage;
    }
    else if (uEventHash == FNV1A::HashConst("round_officially_ended")) {
        // Reset all of the Damage this round counters, as the round has ended
        for (size_t i = 0; i < g_arrDataPlayers.size(); i++)
        {
            PlayerData* pData = g_arrDataPlayers[i];

            if (!pData)
                continue;

            pData->m_nDamageThisRound = 0;
        }
    }

}

namespace HookFuncs {
	HRESULT Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
		// Check if we have a render target
		if (!CSData::g_pRenderTarget) {
			// Create a render target for the SwapChain's backbuffer
			ID3D11Texture2D* pBackBuffer = nullptr;
			CSData::g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
			CSData::g_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, (ID3D11RenderTargetView**)&CSData::g_pRenderTarget);
			MEM::Msg(MEM::AcquireChannel("FragFrame"), ("Created Render Target: " + int_to_hex((uintptr_t)CSData::g_pRenderTarget)).c_str(), CS_CLR_GREEN);
			pBackBuffer->Release();

			// Get the SwapChain's Description and extract the window handle
			DXGI_SWAP_CHAIN_DESC Desc{};
			pSwapChain->GetDesc(&Desc);
			CSData::g_Window = Desc.OutputWindow;

			// Initialize ImGui
			ImGui::CreateContext();
			ImGui_ImplDX11_Init(CSData::g_pDevice, CSData::g_pContext);
			ImGui_ImplWin32_Init(CSData::g_Window);
			MEM::Msg(MEM::AcquireChannel("FragFrame"), "Backend initialized", CS_CLR_GREEN);

			// Get the ImGui context
			ImGuiIO& io = ImGui::GetIO();

			// Set the ImGui DisplaySize to the size of the back buffer
			io.DisplaySize = ImVec2{ (float)Desc.BufferDesc.Width, (float)Desc.BufferDesc.Height };

			ImFontConfig FontAwesomeConfig{};
			FontAwesomeConfig.MergeMode = true;
			FontAwesomeConfig.FontDataOwnedByAtlas = true;
			FontAwesomeConfig.GlyphRanges = new ImWchar[]{ ICON_MIN_FA, ICON_MAX_FA, 0 };

			// Load the fonts
			Fonts::g_pDefaultSmall = io.Fonts->AddFontFromFileTTF(FF_PATH"fonts\\arial.ttf", 32.f, nullptr);
			Fonts::g_pDefaultLarge = io.Fonts->AddFontFromFileTTF(FF_PATH"fonts\\arial.ttf", 128.f, nullptr);
			Fonts::g_pDefaultLargeItalic = io.Fonts->AddFontFromFileTTF(FF_PATH"fonts\\ariali.ttf", 128.f, nullptr);
			Fonts::g_pDefaultSmallItalic = io.Fonts->AddFontFromFileTTF(FF_PATH"fonts\\ariali.ttf", 32.f, nullptr);
			Fonts::g_pDefaultSmallBold = io.Fonts->AddFontFromFileTTF(FF_PATH"fonts\\arialbd.ttf", 32.f, nullptr);
			Fonts::g_pFontAwesome = io.Fonts->AddFontFromFileTTF(FF_PATH"fonts\\fa-solid-900.ttf", 17.f, &FontAwesomeConfig);
			io.Fonts->Build(); // Build the font atlas

			if(Fonts::g_pDefaultLarge == nullptr || Fonts::g_pDefaultSmall == nullptr || Fonts::g_pFontAwesome == nullptr)
				MEM::Msg(MEM::AcquireChannel("FragFrame"), "Failed to load one or more fonts, is FragFrame installed correctly?", CS_CLR_RED);
			else {
				// I dont like logging :(
				MEM::Msg(MEM::AcquireChannel("FragFrame"), ("Fonts loaded. (g_pDefaultSmall: " + int_to_hex((uintptr_t)Fonts::g_pDefaultSmall) + 
					", g_pDefaultLarge: " + int_to_hex((uintptr_t)Fonts::g_pDefaultLarge) + ", g_pFontAwesome: " + 
					int_to_hex((uintptr_t)Fonts::g_pFontAwesome) + ")").c_str(), CS_CLR_WHITE);
			}
			MEM::Msg(MEM::AcquireChannel("FragFrame"), "Renderer Backend initialized", CS_CLR_GREEN);
		}

        // Calulcate card scale -- FIXME
        {
            float minDisplayWidth = 1920.0f;
            float maxDisplayWidth = 3840.0f;
            float minScaling = 1.f;
            float maxScaling = 2.f;
            // Get the current display width
            RECT clientRect;
            GetClientRect(CSData::g_Window, &clientRect);
            float displayWidth = (float)(clientRect.right - clientRect.left);

            // Calculate the scaling factor using linear interpolation
            flCardScaling = minScaling + ((displayWidth - minDisplayWidth) / (maxDisplayWidth - minDisplayWidth)) * (maxScaling - minScaling);

            // Clamp the scaling factor to ensure it stays within the desired range>
            flCardScaling = std::clamp(flCardScaling, minScaling, maxScaling);
        }

		// Tell ImGui to clear the previous frame's draw data and start over
		ImGui::NewFrame();
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();

		if (CSData::g_pGameRules && CSData::g_pLocalController && CSData::g_pLocalController->m_iTeamNum() == TEAM_SPECTATOR && !CSData::g_pGameRules->m_bWarmupPeriod()) {
            ImDrawList* pDrawList = ImGui::GetBackgroundDrawList();

            int nRound = CSData::g_pGameRules->GetRound();
            int nFreezeTimeLeft = CSData::g_pGameRules->GetFreezeTimeLeft();
            if (nRound == 1 && nFreezeTimeLeft <= 5 && !FFState::g_StartAnimation.m_bPlaying) {
                FFState::g_StartAnimation.m_bPlaying = true;
                FFState::g_StartAnimation.m_flProgress = 0.f;
                FFState::g_StartAnimation.m_flStart = 0.f;
                FFState::g_StartAnimation.m_flEnd = 10.f;
                FFState::g_StartAnimation.m_flSpeed = .16f; // for the love of god dont change this
                PlaySoundA(FF_PATH"sfx\\showdown.wav", NULL, SND_FILENAME | SND_ASYNC);
            }
            else {
                std::vector<PlayerData*> team1Players{};
                std::vector<PlayerData*> team2Players{};

                for (size_t i = 0; i < g_arrDataPlayers.size(); i++)
                {
                    if (g_arrDataPlayers[i]->m_nTeam == 0)
                        team1Players.push_back(g_arrDataPlayers[i]);
                    else if (g_arrDataPlayers[i]->m_nTeam == 1)
                        team2Players.push_back(g_arrDataPlayers[i]);
                }

                float flPlayerCardHeight = g_flPlayerCardHeight * flCardScaling;

                float flCardOffset = ImGui::GetIO().DisplaySize.y - 6.f - flPlayerCardHeight;
                float flDeltaTime = ImGui::GetIO().DeltaTime;

                PlayerData* pSpec = nullptr;
                if (CSData::g_pLocalController) {
                    C_BasePlayerPawn* pLocalPawn = CSData::g_pGameEntitySystem->Get<C_BasePlayerPawn>(CSData::g_pLocalController->m_hPawn());
                    if (pLocalPawn) {
                        CPlayer_ObserverServices* pObserverServices = pLocalPawn->m_pObserverServices();
                        if (pObserverServices) {
                            CBaseHandle hObserverTarget = pObserverServices->m_hObserverTarget();
                            C_BaseEntity* pBaseEntity = CSData::g_pGameEntitySystem->Get<C_BaseEntity>(hObserverTarget);
                            
                            if (pBaseEntity) {
                                SchemaClassInfoData_t* pSchema = nullptr;
                                pBaseEntity->GetSchemaClassInfo(&pSchema);
                                FNV1A_t uTargetNameHash = FNV1A::Hash(pSchema->szName);

                                if (uTargetNameHash == FNV1A::HashConst("C_CSPlayerPawn")) {
                                    pSpec = GetPlayerDataFromPawn((C_CSPlayerPawn*)pBaseEntity);
                                }
                            }
                        }
                    }
                }

                for (size_t pid = 0; pid < team1Players.size(); pid++)
                {
                    flCardOffset -= DrawPlayerCard(team1Players[pid], flDeltaTime, pDrawList, flCardOffset, pSpec ? team1Players[pid]->m_hPawn == pSpec->m_hPawn : false);
                }

                flCardOffset = ImGui::GetIO().DisplaySize.y - 6.f - flPlayerCardHeight;

                for (size_t pid = 0; pid < team2Players.size(); pid++)
                {
                    flCardOffset -= DrawPlayerCard(team2Players[pid], flDeltaTime, pDrawList, flCardOffset, pSpec ? team2Players[pid]->m_hPawn == pSpec->m_hPawn : false);
                }

                // Had these hardcoded, oopsies!
                // Funny fun fact, if nobody ever joined T or CT team, then it will be nullptr and fuck everything up :3
                const char* szTerrorTeamName = CSData::g_pTTeam->m_szTeamname();
                const char* szCounterTerrorTeamName = CSData::g_pCTTeam->m_szTeamname();

                DrawHeader(pDrawList, szTerrorTeamName, szCounterTerrorTeamName);
                if(pSpec)
                    DrawSpecPlayer(pDrawList, pSpec, pSpec->m_nTeam == 0 ? szTerrorTeamName : szCounterTerrorTeamName, pSpec->m_nTeam == 0 ? Theme::colTerroristsColor : Theme::colCounterTerroristsColor);

                DrawPanorama(pDrawList);
            }

            if (CSData::g_pGameRules->m_bFreezePeriod() && nFreezeTimeLeft > 3 && nRound == 1) {
                static int nTransitImageIndex = -1;
                static int nImageIndex = 0;
                static float flLastImageUpdateTime = FLT_MIN;
                static CAnimation TransitImageAnim{};

                FNV1A_t uMapHash = FNV1A::Hash(CSData::g_pGlobalVars->GetMapName());

                ID3D11ShaderResourceView* pSRV = nullptr;
                ID3D11ShaderResourceView* pTransitSRV = nullptr;

                if (g_mImagesForMaps.contains(uMapHash)) {
                    float flTimePassed = fabsf(CSData::g_pGlobalVars->m_flCurrentTime - flLastImageUpdateTime);
                    if (flTimePassed > 7.5f) {
                        flLastImageUpdateTime = CSData::g_pGlobalVars->m_flCurrentTime;
                        TransitImageAnim.m_flProgress = 0.f;
                        TransitImageAnim.m_bPlaying = true;
                        TransitImageAnim.m_flEnd = 0.f;
                        TransitImageAnim.m_flStart = 0.f;
                        TransitImageAnim.m_flSpeed = .8f;
                        nTransitImageIndex = nImageIndex + 1;
                        if (nTransitImageIndex >= g_mImagesForMaps[uMapHash].size())
                            nTransitImageIndex = 0;
                    }

                    if (TransitImageAnim.m_flProgress >= 1.f) {
                        TransitImageAnim.m_bPlaying = false;
                        TransitImageAnim.m_flProgress = 0.f;
                        nImageIndex = nTransitImageIndex;
                        nTransitImageIndex = -1;
                    }

                    if (nTransitImageIndex != -1)
                        pTransitSRV = g_mImagesForMaps[uMapHash][nTransitImageIndex];
                    pSRV = g_mImagesForMaps[uMapHash][nImageIndex];
                }

                if (TransitImageAnim.m_bPlaying)
                    TransitImageAnim.Tick();

                pDrawList->AddImage((ImTextureID)pSRV, ImVec2{}, ImGui::GetIO().DisplaySize);

                if(pTransitSRV)
                    pDrawList->AddImage((ImTextureID)pTransitSRV, ImVec2{}, ImGui::GetIO().DisplaySize, ImVec2{0.f, 0.f}, ImVec2{1.f, 1.f}, ImColor(1.f, 1.f, 1.f, TransitImageAnim.m_flProgress));

                pDrawList->AddRectFilled(ImVec2{}, ImGui::GetIO().DisplaySize, ImColor(23, 23, 23, 170));
                std::string topText = "The Match will start in";
                float ttWidth = Fonts::g_pDefaultLargeItalic->CalcTextSizeA(96.f, FLT_MAX, FLT_MAX, topText.c_str()).x;
                pDrawList->AddText(Fonts::g_pDefaultLargeItalic, 96.f, ImVec2{ (ImGui::GetIO().DisplaySize.x - ttWidth) * 0.5f, (ImGui::GetIO().DisplaySize.y / 2.f) - 128.f - 32.f }, ImColor(245, 245, 245), topText.c_str());
                std::string timeText = std::to_string(nFreezeTimeLeft) + " seconds";
                float ttiWidth = Fonts::g_pDefaultLarge->CalcTextSizeA(128.f, FLT_MAX, FLT_MAX, timeText.c_str()).x;
                pDrawList->AddText(Fonts::g_pDefaultLarge, 128.f, ImVec2{ (ImGui::GetIO().DisplaySize.x - ttiWidth) * 0.5f, (ImGui::GetIO().DisplaySize.y / 2.f) }, ImColor(245, 245, 245), timeText.c_str());
            }

            if (FFState::g_StartAnimation.m_bPlaying)
                FFState::g_StartAnimation.Tick();

            if (FFState::g_StartAnimation.m_bPlaying) {
                float value = FFState::g_StartAnimation.GetValue();

                static float flFontSize = 128.f;

                if (value != 10.f) {
                    if (value > 2.f && value < 8.f) {
                        pDrawList->AddRectFilled(ImVec2{ 0.f, 0.f }, ImGui::GetIO().DisplaySize, ImColor(12, 12, 12)); // Make this less boring
                        int numEntries = 0;
                        float svalue = value;
                        // Lord have mercy
                        if (svalue >= 3.9f && svalue < 4.1f)
                            numEntries = 1;
                        else if (svalue > 4.1f && svalue < 4.2f)
                            numEntries = 2;
                        else if (svalue >= 4.2f && svalue < 4.4f)
                            numEntries = 3;
                        else if (svalue >= 4.4f && svalue < 4.6f)
                            numEntries = 4;
                        else if (svalue >= 4.6f && svalue < 4.8f)
                            numEntries = 5;
                        else if (svalue >= 4.8f && svalue < 5.f)
                            numEntries = 6;
                        else if (svalue >= 5.f && svalue < 5.2f)
                            numEntries = 7;
                        else if (svalue >= 5.2f && svalue < 5.4f)
                            numEntries = 8;
                        else if (svalue >= 5.4f && svalue < 5.6f)
                            numEntries = 9;
                        else if (svalue >= 5.6f && svalue < 5.8f)
                            numEntries = 10;
                        else if (svalue >= 5.8f && svalue < 6.f)
                            numEntries = 11;
                        else if (svalue >= 6.f && svalue < 6.2f)
                            numEntries = 12;
                        else if (svalue >= 6.2f && svalue < 6.4f)
                            numEntries = 13;
                        else if (svalue >= 6.4f && svalue < 6.6f)
                            numEntries = 14;
                        else if (svalue >= 6.6f && svalue < 6.8f)
                            numEntries = 15;
                        else if (svalue >= 6.8f && svalue < 7.f)
                            numEntries = 16;
                        else if (svalue >= 7.f && svalue < 7.2f)
                            numEntries = 17;
                        else if (svalue >= 7.2f && svalue < 7.4f)
                            numEntries = 18;
                        else if (svalue >= 7.4f)
                            numEntries = 19;
                        else if (svalue < 3.9f)
                            numEntries = -1;
                        else
                            numEntries = 0;

                        ImColor color = ImColor(255, 255, 255);
                        if (svalue > 5.2f) {
                            // Calculate the interpolation factor (0 to 1) based on svalue
                            float steps = 8.0f; // Number of steps
                            float stepSize = 0.2f; // Increment per step
                            float startValue = 5.2f; // Starting value for the transition

                            // Clamp svalue to the range [startValue, startValue + steps * stepSize]
                            float clampedValue = (float)(numEntries - 8);

                            // Calculate the interpolation factor (0 to 1)
                            float t = clampedValue / 9.f;

                            // Interpolate between white (255, 255, 255) and orange (209, 134, 0)
                            ImColor white = ImColor(255, 255, 255);
                            ImColor orange = ImColor(209, 134, 0);

                            color = ImColor(
                                Lerp(white.Value.x, orange.Value.x, t),
                                Lerp(white.Value.y, orange.Value.y, t),
                                Lerp(white.Value.z, orange.Value.z, t)
                            );
                        }

                        // Calculate the total height required for all text entries
                        float totalTextHeight = (numEntries > 9 ? 9 : numEntries) * flFontSize;

                        // Calculate the starting Y position to center the text vertically
                        float startY = (ImGui::GetIO().DisplaySize.y - totalTextHeight) * 0.5f;

                        // Set the initial position for the first text entry
                        ImVec2 vecPos = ImVec2{ (ImGui::GetIO().DisplaySize.x * 0.5f), startY };

                        if (numEntries == -1) {
                            startY = (ImGui::GetIO().DisplaySize.y - flFontSize) * 0.5f;
                            vecPos = ImVec2{ (ImGui::GetIO().DisplaySize.x * 0.5f), startY };
                            float setWidth = Fonts::g_pDefaultLarge->CalcTextSizeA(flFontSize, FLT_MAX, FLT_MAX, "SET!").x;
                            pDrawList->AddText(
                                Fonts::g_pDefaultLargeItalic,
                                flFontSize,
                                ImVec2(vecPos.x - setWidth * 0.5f, vecPos.y), // Adjust X and Y
                                ImColor(255, 255, 255),
                                "SET"
                            );
                        }

                        float goWidth = Fonts::g_pDefaultLarge->CalcTextSizeA(flFontSize, FLT_MAX, FLT_MAX, "GO!").x;
                        if (numEntries <= 9) {
                            // Render each text entry
                            for (int i = 0; i < numEntries; ++i) {
                                // Center the text horizontally and adjust the Y position for each entry
                                pDrawList->AddText(
                                    Fonts::g_pDefaultLargeItalic,
                                    flFontSize,
                                    ImVec2(vecPos.x - goWidth * 0.5f, vecPos.y + i * flFontSize), // Adjust X and Y
                                    color,
                                    "GO!"
                                );
                            }
                        }
                        else {
                            // Calculate the number of additional "GO!" bars to add on the sides
                            int numAdditionalBars = numEntries - 8;
                            float spacing = goWidth + 8.f; // Spacing between "GO!" bars

                            // Render the center "GO!" bars (5 in total)
                            for (int i = 0; i < 9; ++i) {
                                pDrawList->AddText(
                                    Fonts::g_pDefaultLargeItalic,
                                    flFontSize,
                                    ImVec2(vecPos.x - goWidth * 0.5f, vecPos.y + i * flFontSize), // Center horizontally
                                    color,
                                    "GO!"
                                );
                            }

                            // Render additional "GO!" bars on the left and right sides
                            for (int side = 0; side < 2; ++side) { // 0 = left, 1 = right
                                for (int bar = 1; bar <= numAdditionalBars; ++bar) {
                                    float xOffset = (side == 0) ? -(bar * spacing) : (bar * spacing);

                                    for (int i = 0; i < 9; ++i) {
                                        pDrawList->AddText(
                                            Fonts::g_pDefaultLargeItalic,
                                            flFontSize,
                                            ImVec2(vecPos.x - goWidth * 0.5f + xOffset, vecPos.y + i * flFontSize), // Center horizontally
                                            color,
                                            "GO!"
                                        );
                                    }
                                }
                            }
                        }
                    }
                    else if (value > 8.f) {
                        float prog = 1.f - ((value - 8.f) / 2.f);
                        int alpha = (int)(255 * prog);

                        pDrawList->AddRectFilled(ImVec2{ 0.f, 0.f }, ImGui::GetIO().DisplaySize, ImColor(12, 12, 12, alpha)); // Make this less boring
                        // Calculate the number of additional "GO!" bars to add on the sides
                        float goWidth = Fonts::g_pDefaultLarge->CalcTextSizeA(flFontSize, FLT_MAX, FLT_MAX, "GO!").x;

                        // Calculate the total height required for all text entries
                        float totalTextHeight = 7 * flFontSize;

                        // Calculate the starting Y position to center the text vertically
                        float startY = (ImGui::GetIO().DisplaySize.y - totalTextHeight) * 0.5f;

                        // Set the initial position for the first text entry
                        ImVec2 vecPos = ImVec2{ (ImGui::GetIO().DisplaySize.x * 0.5f), startY };

                        // Calculate the number of additional "GO!" bars to add on the sides
                        int numAdditionalBars = 11;
                        float spacing = goWidth + 8.f; // Spacing between "GO!" bars

                        // Force color to orange
                        ImColor color = ImColor(209, 134, 0);

                        // Render the center "GO!" bars (5 in total)
                        for (int i = 0; i < 9; ++i) {
                            pDrawList->AddText(
                                Fonts::g_pDefaultLargeItalic,
                                flFontSize,
                                ImVec2(vecPos.x - goWidth * 0.5f, vecPos.y + i * flFontSize), // Center horizontally
                                color,
                                "GO!"
                            );
                        }

                        // Render additional "GO!" bars on the left and right sides
                        for (int side = 0; side < 2; ++side) { // 0 = left, 1 = right
                            for (int bar = 1; bar <= numAdditionalBars; ++bar) {
                                float xOffset = (side == 0) ? -(bar * spacing) : (bar * spacing);

                                for (int i = 0; i < 9; ++i) {
                                    pDrawList->AddText(
                                        Fonts::g_pDefaultLargeItalic,
                                        flFontSize,
                                        ImVec2(vecPos.x - goWidth * 0.5f + xOffset, vecPos.y + i * flFontSize), // Center horizontally
                                        color,
                                        "GO!"
                                    );
                                }
                            }
                        }
                    }
                    else if (value < 2.f) {
                        float prog = value / 2.f;
                        int alpha = (int)(255 * prog);

                        // Calculate the starting Y position to center the text vertically
                        float startY = (ImGui::GetIO().DisplaySize.y - flFontSize) * 0.5f;

                        // Set the initial position for the first text entry
                        ImVec2 vecPos = ImVec2{ (ImGui::GetIO().DisplaySize.x * 0.5f), startY };
                        float readyWidth = Fonts::g_pDefaultLarge->CalcTextSizeA(flFontSize, FLT_MAX, FLT_MAX, "READY?").x;

                        pDrawList->AddRectFilled(ImVec2{ 0.f, 0.f }, ImGui::GetIO().DisplaySize, ImColor(12, 12, 12, alpha));
                        pDrawList->AddText(
                            Fonts::g_pDefaultLargeItalic,
                            flFontSize,
                            ImVec2(vecPos.x - readyWidth * 0.5f, vecPos.y), // Adjust X and Y
                            ImColor(255, 255, 255, alpha),
                            "READY?"
                        );
                    }
                }
            }
		}

		// Render the current frame
		ImGui::Render();

		// Set the Render Target, because the game's rendering ends with some other Render Target being binded
		CSData::g_pContext->OMSetRenderTargets(1, &CSData::g_pRenderTarget, nullptr);

		// And send the draw commands to the GPU
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        if (!g_pHooks->Present.IsOk())
            return S_OK;

		// Call the original present
		using fnOriginal_t = HRESULT(*)(IDXGISwapChain*, UINT, UINT);
		static fnOriginal_t fnOriginal = reinterpret_cast<fnOriginal_t>(g_pHooks->Present.GetOriginal()); // Static to save performance (even tho its just a few CPU cycles)
		return fnOriginal(pSwapChain, SyncInterval, Flags);
	}

    bool __fastcall MouseInputEnabled(void* pThis)
    {
        if (Theme::bAllowMouse) {
            return false;
        }
        else {
            if (!g_pHooks->MouseInputEnabled.IsOk())
                return false;

            using fnOriginal_t = bool(__fastcall*)(void* pThis);
            static fnOriginal_t fnOriginal = reinterpret_cast<fnOriginal_t>(g_pHooks->MouseInputEnabled.GetOriginal());
            return fnOriginal(pThis);
        }
    }

	__int64* __fastcall LevelInit(void* pClientModeShared, const char* szNewMap) {
        // These change every time we load a map (I'm not sure about the entity system, better safe than sorry)
		CSData::g_arrPlayers = std::vector<C_CSPlayerPawn*>{};
		CSData::g_pGameEntitySystem = *(CGameEntitySystem**)MEM::GetAbsoluteAddress(MEM::PatternScan(MEM::GetModuleBaseHandle("client.dll"), "48 8B 0D ? ? ? ? 0F 57 C0 C7 44 24 30 ? ? ? ?"), 0x3);
		CSData::g_pGameRules = *(C_CSGameRules**)MEM::GetAbsoluteAddress(MEM::PatternScan(MEM::GetModuleBaseHandle("client.dll"), "48 8B 1D ? ? ? ? 80 7B 41 00"), 0x3);
		CSData::g_pGlobalVars = *(IGlobalVars**)MEM::GetAbsoluteAddress(MEM::PatternScan(MEM::GetModuleBaseHandle("client.dll"), "48 89 15 ? ? ? ? 48 89 42"), 0x3);

        FFState::g_StartAnimation.m_bPlaying = false; // Easiest way to reset it

        CSData::g_pEngineCVar->Find(FNV1A::HashConst("cl_obs_interp_enable"))->value.i1 = false; // Disable interpolation on the Observer's camera
        CSData::g_pEngineCVar->Find(FNV1A::HashConst("cl_draw_only_deathnotices"))->value.i1 = true; // Only render the deathnotices, and not the entire HUD
        CSData::g_pEngineCVar->Find(FNV1A::HashConst("cl_drawhud_force_teamid_overhead"))->value.i1 = true; // Force it to true to keep the overhead names
        CSData::g_pEngineCVar->Find(FNV1A::HashConst("cl_teamid_overhead_mode"))->value.i32 = 2; // Only show name and HP

        // We use our own crosshair, don't render the person's one
        CSData::g_pEngineCVar->Find(FNV1A::HashConst("cl_show_observer_crosshair"))->value.i32 = 0;
        CSData::g_pEngineCVar->Find(FNV1A::HashConst("crosshair"))->value.i1 = false;

        if (!g_pHooks->LevelInit.IsOk())
            return nullptr;

		// Call the original LevelInit
		using fnOriginal_t = __int64*(__fastcall*)(void* pClientModeShared, const char* szNewMap);
		static fnOriginal_t fnOriginal = reinterpret_cast<fnOriginal_t>(g_pHooks->LevelInit.GetOriginal()); // Static to save performance (even tho its just a few CPU cycles)
		return fnOriginal(pClientModeShared, szNewMap);
	}

	__int64 __fastcall LevelShutdown(void* pClientModeShared) {
        // Null out all of the fields
		CSData::g_arrPlayers = std::vector<C_CSPlayerPawn*>{};
        g_arrDataPlayers.clear();
		CSData::g_pGameEntitySystem = nullptr;
		CSData::g_pGameRules = nullptr;
		CSData::g_pGlobalVars = nullptr;
		CSData::g_pLocalController = nullptr;
        CSData::g_pTTeam = nullptr;
        CSData::g_pCTTeam = nullptr;
        CSData::g_arrTeams.clear();

        if (!g_pHooks->LevelShutdown.IsOk())
            return 0;

		// Call the original LevelInit
		using fnOriginal_t = __int64 (__fastcall*)(void* pClientModeShared);
		static fnOriginal_t fnOriginal = reinterpret_cast<fnOriginal_t>(g_pHooks->LevelShutdown.GetOriginal()); // Static to save performance (even tho its just a few CPU cycles)
		return fnOriginal(pClientModeShared);
	}

	__int64 __fastcall UpdateTimerLabel(__int64 pHudTeamCounter) {
		// I wont even begin to claim whats going on here, but it works for getting round time / freeze time left

        if (!g_pHooks->UpdateTimerLabel.IsOk())
            return 0;

		using fnOriginal_t = __int64 (__fastcall*)(__int64);
		static fnOriginal_t fnOriginal = reinterpret_cast<fnOriginal_t>(g_pHooks->UpdateTimerLabel.GetOriginal());
		__int64 Result = fnOriginal(pHudTeamCounter);

        // What in gods name is this piece of shit
		CSIData::m_nHudTeamCounterLabel = *(int*)(pHudTeamCounter + 0x20D0);
		return Result;
	}

	void __fastcall OnAddEntity(void* pThis, C_BaseEntity* pEntity, CBaseHandle hHandle) {
		if (pEntity->IsPlayerPawn()) {
			C_CSPlayerPawn* pPawn = (C_CSPlayerPawn*)pEntity;
			CSData::g_arrPlayers.push_back(pPawn);

            PlayerData* pData = new PlayerData{};
            // Set this shit in FSN, controller is nullptr when it's added for some fucking reason...
            pData->m_hController = CBaseHandle(0);
            pData->m_szName = nullptr;

            // how about we get ZeroMemory or memset in this fucker?
            pData->m_hPawn = hHandle;
            pData->m_iDeaths = 0;
            pData->m_iKills = 0;
            pData->m_iMoney = 0;
            pData->m_nTeam = -1;
            pData->m_nDamageThisRound = 0;
            pData->m_AnimationData.m_flRenderHealth = 0.f;
            pData->m_AnimationData.m_flTargetHealth = 0.f;
            pData->m_WeaponData.m_arrGrenades = {};
            pData->m_WeaponData.m_pPrimary = nullptr;
            pData->m_WeaponData.m_pSecondary = nullptr;
            pData->m_WeaponData.m_bHasHelmet = false;
            pData->m_WeaponData.m_bHasArmor = false;
            g_arrDataPlayers.push_back(pData);
		}

        if (!g_pHooks->OnAddEntity.IsOk())
            return;

        SchemaClassInfoData_t* pSchema = nullptr;
        pEntity->GetSchemaClassInfo(&pSchema);
        if (pSchema) {
            if (pSchema->InheritsFrom(FNV1A::HashConst("C_Team"))) {
                // Honestly, IDK why we're doing it like this, but i couldn't figure out another way
                CSData::g_arrTeams.push_back((C_Team*)pEntity);
            }
        }

		using fnOriginal_t = void(__fastcall*)(void* pThis, C_BaseEntity* pEntity, CBaseHandle hHandle);
		static fnOriginal_t fnOriginal = reinterpret_cast<fnOriginal_t>(g_pHooks->OnAddEntity.GetOriginal());
		fnOriginal(pThis, pEntity, hHandle);
	}

	void __fastcall OnRemoveEntity(void* pThis, C_BaseEntity* pEntity, CBaseHandle hHandle) {
		if (pEntity->IsPlayerPawn()) {
            // TODO: make this code prettier (no seriously, wtf is this?)
			size_t nIndex = MAXSSIZE_T;

            // Find the fuck
			for (size_t i = 0; i < CSData::g_arrPlayers.size(); i++)
			{
				if(CSData::g_arrPlayers[i]->m_hEntityHandle() == hHandle) {
					nIndex = i;
					break;
				}
			}

            // Erase the fuck (if we found it)
			if (nIndex != MAXSSIZE_T)
				CSData::g_arrPlayers.erase(CSData::g_arrPlayers.begin() + nIndex);
		}

        if (!g_pHooks->OnRemoveEntity.IsOk())
            return;

		using fnOriginal_t = void(__fastcall*)(void* pThis, C_BaseEntity* pEntity, CBaseHandle hHandle);
		static fnOriginal_t fnOriginal = reinterpret_cast<fnOriginal_t>(g_pHooks->OnRemoveEntity.GetOriginal());
		fnOriginal(pThis, pEntity, hHandle);
	}

    char __fastcall FireClientEvent(void* pThis, IGameEvent* pEvent) {
        // This code was so ugly, I moved it to its own function so that I don't have to look at it.
        if (pEvent)
            HandleGameEvent(pEvent);

        if (!g_pHooks->FireClientEvent.IsOk())
            return 0;

        using fnOriginal_t = char(__fastcall*)(void* pThis, IGameEvent* pEvent);
        static fnOriginal_t fnOriginal = reinterpret_cast<fnOriginal_t>(g_pHooks->FireClientEvent.GetOriginal());
        return fnOriginal(pThis, pEvent);
    }

    void __fastcall FrameStageNotify(void* pThis, int nStage) {
        if (nStage == FRAME_NET_UPDATE_END) {
            // We have received all of the packets and updates from the Server, all of the data is up to date now
            // Let's update all of the player's weapons, health and other data
            for (size_t i = 0; i < g_arrDataPlayers.size(); i++)
            {
                PlayerData* pData = g_arrDataPlayers[i];
                if (!pData)
                    continue;

                if (pData->m_hController.m_iEntryIndex() == 0) {
                    // Controller is empty, we gotta set it here!

                    C_CSPlayerPawn* pPawn = CSData::g_pGameEntitySystem->Get<C_CSPlayerPawn>(pData->m_hPawn);
                    if (!pPawn)
                        continue;

                    CCSPlayerController* pController = CSData::g_pGameEntitySystem->Get<CCSPlayerController>(pPawn->m_hController());
                    if (!pController)
                        continue;
                     
                    if (pController->m_bIsLocalPlayerController())
                        continue;

                    // Set the 2 vars we can't set in OnAddEntity
                    pData->m_hController = pPawn->m_hController();
                    pData->m_szName = _strdup(pController->m_sSanitizedPlayerName());
                    // Acknowledge the player in the console too
                    MEM::Msg(MEM::AcquireChannel("FragFrame"), ("Added player \"" + std::string(pData->m_szName) + "\"").c_str(), CS_CLR_WHITE);
                }

                CCSPlayerController* pController = CSData::g_pGameEntitySystem->Get<CCSPlayerController>(pData->m_hController);
                if (!pController)
                    continue;

                if (pController->m_bIsLocalPlayerController())
                    continue;

                C_CSPlayerPawn* pPawn = CSData::g_pGameEntitySystem->Get<C_CSPlayerPawn>(pData->m_hPawn);
                if (!pPawn)
                    continue;

                CCSPlayerController_InGameMoneyServices* pMoneyServices = pController->m_pInGameMoneyServices();
                if (!pMoneyServices)
                    continue;

                // We use our own indices for teams
                // 
                // Getting g_pTTeam and g_pCTTeam by using the player controllers on that Team, might be the most retarded thing in this entire project,
                // but it works (most of the time), so we'll just run with it.

                if (pController->m_iTeamNum() == TEAM_TT) {
                    pData->m_nTeam = 0;

                    if (!CSData::g_pTTeam) {
                        for (size_t i = 0; i < CSData::g_arrTeams.size(); i++)
                        {
                            if (CSData::g_arrTeams[i]->m_aPlayerControllers().Contains(pData->m_hController)) {
                                CSData::g_pTTeam = CSData::g_arrTeams[i];
                                MEM::Msg(MEM::AcquireChannel("FragFrame"), ("Found Terrorist Team. (Pointer: " + int_to_hex((uintptr_t)CSData::g_pTTeam) + ")").c_str(), CS_CLR_WHITE);
                                break;
                            }
                        }
                    }
                }
                else if (pController->m_iTeamNum() == TEAM_CT) {
                    pData->m_nTeam = 1;

                    if (!CSData::g_pCTTeam) {
                        for (size_t i = 0; i < CSData::g_arrTeams.size(); i++)
                        {
                            if (CSData::g_arrTeams[i]->m_aPlayerControllers().Contains(pData->m_hController)) {
                                CSData::g_pCTTeam = CSData::g_arrTeams[i];
                                MEM::Msg(MEM::AcquireChannel("FragFrame"), ("Found Counter-Terrorist Team. (Pointer: " + int_to_hex((uintptr_t)CSData::g_pCTTeam) + ")").c_str(), CS_CLR_WHITE);
                                break;
                            }
                        }
                    }
                }
                else
                    pData->m_nTeam = -1;

                // Update health and money
                pData->m_iHealth = pPawn->m_iHealth();
                pData->m_AnimationData.m_flTargetHealth = (float)pData->m_iHealth;
                pData->m_iMoney = pMoneyServices->m_iAccount();
                pData->m_nKillsThisRound = pController->m_nKillCount(); // This is per round for some reason

                CCSPlayer_WeaponServices* pWeaponServices = pPawn->m_pWeaponServices();
                if (!pWeaponServices)
                    continue;

                // Probably the most inefficient way to do this, no checks if weapons changed?
                C_NetworkUtlVectorBase<CBaseHandle> arrWeapons = pWeaponServices->m_hMyWeapons();
                PlayerWeaponData WeaponData{};
                for (size_t i = 0; i < arrWeapons.nSize; i++)
                {
                    C_CSWeaponBaseGun* pWeapon = CSData::g_pGameEntitySystem->Get<C_CSWeaponBaseGun>(arrWeapons.pElements[i]);
                    if (!pWeapon)
                        continue;
                    
                    if (pWeapon->m_WpnVData()->IsPrimary()) {
                        // Primary
                        WeaponData.m_pPrimary = LoadWeapon(pWeapon);
                    }
                    else if (pWeapon->m_WpnVData()->m_WeaponType() == WEAPONTYPE_PISTOL) {
                        // Secondary
                        WeaponData.m_pSecondary = LoadWeapon(pWeapon);
                    }
                    else if (pWeapon->m_WpnVData()->m_WeaponType() == WEAPONTYPE_GRENADE) {
                        // Grenade
                        // I honestly don't know why its not a vector of Weapon*, but instead a vector of Weapon
                        // I wrote this shitty vector in the imgui_examples solution at 2 am, cut me some slack
                        
                        // Also we fully rely on the fact that m_hMyWeapons doesnt change the nades around
                        // as we don't sort them, however in my testing this is fine.
                        // If anybody wants to implementing some sort of sorting, that'd be just fine by me.
                        WeaponData.m_arrGrenades.push_back(*LoadWeapon(pWeapon));
                    }
                }

                // Update armor and helmet
                WeaponData.m_bHasHelmet = pController->m_bPawnHasHelmet();
                WeaponData.m_bHasArmor = pPawn->m_ArmorValue() > 0;

                // Set the Weapon Data
                pData->m_WeaponData = WeaponData;
            }
        }

        if (CSData::g_pLocalController) {
            C_CSPlayerPawn* pLocalPawn = CSData::g_pGameEntitySystem->Get<C_CSPlayerPawn>(CSData::g_pLocalController->m_hPawn());
            if (pLocalPawn) {
                CPlayer_ObserverServices* pObserverServices = pLocalPawn->m_pObserverServices();

                if (pObserverServices) {
                    if (nStage == FRAME_SIMULATE_END) {
                        if (CSData::g_pTargetSpectatorOverride) {
                            CBaseHandle hTarget = *CSData::g_pTargetSpectatorOverride;

                            // Modifying schema -- Oh noes!
                            pObserverServices->m_hObserverTarget() = hTarget;
                        }
                    }

                    // Roaming isnt 3rdperson, but we treat it like that
                    CSData::g_bSpectatorThirdPerson = pObserverServices->m_iObserverMode() == ObserverMode_t::OBS_MODE_CHASE ||
                                                        pObserverServices->m_iObserverMode() == ObserverMode_t::OBS_MODE_ROAMING;

                    CSData::g_pCurrentSpectatingPlayer = CSData::g_pGameEntitySystem->Get<C_CSPlayerPawn>(pObserverServices->m_hObserverTarget());
                }
            }
        }

        if (CSData::g_bShowStartPopup) {
            CSData::g_bShowStartPopup = false;
            const char* szScript = "UiToolkitAPI.ShowGenericPopup(\"FragFrame\", \"FragFrame ready! Waiting for you to start spectating...\", \"none\")";
            CSData::g_pPanoramaEngine->RunScriptOnMainMenuPanel(szScript);
        }

        // Only reason for this check in any hook, if we hook these funcs at unlucky timings this will fail, so instead of crashing
        // Just skip this call, nobody cares anyway.
        if (!g_pHooks->FrameStageNotify.IsOk())
            return;

        using fnOriginal_t = void(__fastcall*)(void* pThis, int nStage);
        static fnOriginal_t fnOriginal = reinterpret_cast<fnOriginal_t>(g_pHooks->FrameStageNotify.GetOriginal());
        fnOriginal(pThis, nStage);
    }

    LRESULT WindowProc(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam) {
        ImGui_ImplWin32_WndProcHandler(hWindow, uMessage, wParam, lParam);

        if (uMessage == WM_MOUSEMOVE) {
            if (CSData::g_pEngineClient->IsInGame()) {
                if (Theme::bAllowMouse)
                    CSData::g_pInputSystem->SetRelativeMouseMode(false);
            }
        }

        return ::CallWindowProcA(g_pHooks->WindowProcOrg, hWindow, uMessage, wParam, lParam);
    }
}
