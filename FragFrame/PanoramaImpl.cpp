#include "inc.h"
#include "crosshair_png.h"
#include "3rdparty/stbi.h"
std::unordered_map<FNV1A_t, ID3D11ShaderResourceView*> g_mRadarImages;

// Draws a sample crosshair
void DrawCrosshair(ImDrawList* pDrawList) {
	static ID3D11ShaderResourceView* pCrosshair = nullptr;
	if (!pCrosshair) {
		// bad practices, bad practices everywhere
		// ADHERE TO THE NAMING CONVENTION DAMMIT!
		int w, h;
		int comp;
		stbi_uc* data = stbi_load_from_memory(crosshair_png, sizeof(crosshair_png), &w, &h, &comp, STBI_rgb_alpha);

		int size = w * h * 4;

		pCrosshair = CreateTexture(std::vector<uint8_t>(data, data + size), w, h);
		stbi_image_free(data);
	}

	if (CSData::g_bSpectatorThirdPerson)
		return;

	if (!CSData::g_pCurrentSpectatingPlayer)
		return;

	// Get the weapon
	C_CSWeaponBase* pWeapon = CSData::g_pCurrentSpectatingPlayer->GetWeapon();
	if (!pWeapon)
		return;

	// Get the weapon's vdata
	CCSWeaponBaseVData* pVData = pWeapon->m_WpnVData();
	if (!pVData)
		return;

	// Don't show the scope for snipers
	if (pVData->m_WeaponType() == WEAPONTYPE_SNIPER_RIFLE)
		return;

	ImVec2 vecSize = ImVec2{ 50.f, 50.f }; // Scaling this just makes it looks dogshit in my testing
	ImVec2 vecPos = (ImGui::GetIO().DisplaySize / 2.f) - (vecSize / 2.f);
	pDrawList->AddImage((ImTextureID)pCrosshair, vecPos, vecPos + vecSize);
}

struct RadarPair {
	ID3D11ShaderResourceView* pDefault;
	ID3D11ShaderResourceView* pLower;
};

RadarPair GetRadarPair(std::string szMapName) {
	FNV1A_t uDefaultHash = FNV1A::Hash(szMapName.c_str());
	FNV1A_t uLowerHash = FNV1A::Hash((szMapName + "_lower").c_str());

	RadarPair Pair{};
	Pair.pDefault = g_mRadarImages[uDefaultHash];
	if (g_mRadarImages.contains(uLowerHash))
		Pair.pLower = g_mRadarImages[uLowerHash];
	else
		Pair.pLower = nullptr;
	return Pair;
}

void DrawRadar(ImDrawList* pDrawList) {
	bool bOnLower = false; 
	if (CSData::g_pCurrentSpectatingPlayer)
		bOnLower = IsPlayerOnLowerPartOfTheMap(CSData::g_pCurrentSpectatingPlayer);

	RadarPair Pair = GetRadarPair(std::string(CSData::g_pGlobalVars->GetMapName()));

	// All radar images are 1024x1024
	ImVec2 vecRadarSize = ImVec2{ 384.f, 384.f } * flCardScaling;
	ImVec2 vecRadarPos = { 32.f, 32.f };

	// Container
	pDrawList->AddRectFilled(vecRadarPos, vecRadarPos + vecRadarSize, ImColor(0, 0, 0, 150));

	if (bOnLower) {
		// Default Radar
		pDrawList->AddImage((ImTextureID)Pair.pLower, vecRadarPos, vecRadarPos + vecRadarSize);
	}
	else {
		// Default Radar
		pDrawList->AddImage((ImTextureID)Pair.pDefault, vecRadarPos, vecRadarPos + vecRadarSize);
	}
}

void DrawPanorama(ImDrawList* pDrawList) {
	DrawCrosshair(pDrawList);
	DrawRadar(pDrawList);
}