#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include "IconData.h"
#include "3rdparty/imgui/imgui.h"

// predefinitions
class C_CSWeaponBaseGun;
struct Weapon;
struct PlayerData;
class ID3D11ShaderResourceView;

extern std::unordered_map<FNV1A_t, std::vector<ID3D11ShaderResourceView*>> g_mImagesForMaps;
extern std::unordered_map<FNV1A_t, IconData_t> g_mWeaponIcons;
extern std::vector<PlayerData*> g_arrDataPlayers;

PlayerData* GetPlayerDataFromController(CBasePlayerController* pController);
PlayerData* GetPlayerDataFromPawn(C_CSPlayerPawn* pPawn);

ID3D11ShaderResourceView* CreateTexture(std::vector<uint8_t> Data, int nWidth, int nHeight);
std::string TransformTextToUpper(std::string szText);
float CalculateScaledFontSize(ImFont* font, const char* text, float maxWidth, float initialFontSize);

Weapon* LoadWeapon(C_CSWeaponBaseGun* pWeapon);

ImVec2 ScaleToHeight(const ImVec2& vecImageSize, float targetHeight);
void RenderSlidingColorRect(ImDrawList* drawList, ImVec2 topLeft, ImVec2 size, float rounding);

inline float Lerp(float a, float b, float t) {
    return a + t * (b - a);
}

template< typename T >
std::string int_to_hex(T i);

namespace Theme {
    static ImColor colCardBackground = { 17, 10, 7, 195 }; // Background color of the Player cards
    static ImColor colTerroristsColor = { 247, 174, 57 }; // Team color for the Terrorists
    static ImColor colCounterTerroristsColor = { 28, 155, 179 }; // Team color for the Counter-Terrorists
    static ImColor colDefaultText = { 255, 255, 255 }; // Default Color for Text
    static ImColor colStatIcons = { 104, 99, 97 }; // Color for Statistic Icons
    static ImColor colUtilIcons = { 204, 199, 197 }; // Color for Statistic Icons
    static float flCardOffset = 18.f; // X-Offset for the Player Cards
    static float flCardRounding = 4.f; // Rounding for the Player Cards
    static float flCardFontSize = 24.f; // Font Size for the Player Cards

    static bool bAllowMouse = false; // Allows the Mouse Cursor to be used to click on Player cards, etc

    static float flCardSpectatorBorderSize = 1.f; // Size of the animated borders around the currently spectated player's card

    // Isn't this literally just CSS' padding?
    static float flCardInnerOffsetX = 18.f; // Inner X Offset for all of the Elements of the Player Card
    static float flCardInnerOffsetY = 6.f; // Inner Y Offset for all of the Elements of the Player Card

    static float flGlobalAnimationSpeed = 6.f; // Animation speed of everything, cuz im too lazy to make more vars

    static float flHeaderRounding = 8.f; // Rounding for the header background
    static float flRoundTimeFontSize = 40.f; // Font size for the Round time in the Header
    static float flRoundNumberFontSize = 28.f; // Font size for the Round number in the Header
    static float flScoreFontSize = 54.f; // Font size for the Team score in the Header
    static float flTeamNameFontSize = 48.f; // Font size for the Team name in the Header
}

bool IsPlayerOnLowerPartOfTheMap(C_CSPlayerPawn* pPawn);