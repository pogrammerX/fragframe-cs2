#include "inc.h"

void DrawSpecPlayer(ImDrawList* pDrawList, PlayerData* pSpec, const char* szTeamName, ImColor colTeamColor) {
    ImVec2 vecSpecSize = { 423.f * flCardScaling, 172.f * flCardScaling };
    ImVec2 vecPosition = {
        (ImGui::GetIO().DisplaySize.x / 2.f) - (vecSpecSize.x / 2.f),
        ImGui::GetIO().DisplaySize.y - vecSpecSize.y - 48.f
    };
    float flFontSize = Theme::flCardFontSize * flCardScaling;

    // Draw Border
    if (pSpec->m_AnimationData.m_flRenderHealth > 0.f) {
        ImVec2 vecOffset = { Theme::flCardSpectatorBorderSize,Theme::flCardSpectatorBorderSize };
        vecOffset *= flCardScaling;

        RenderSlidingColorRect(pDrawList, vecPosition - vecOffset, vecSpecSize + (vecOffset * 2.f), 8.f);
    }
    else {
        ImVec2 vecOffset = { Theme::flCardSpectatorBorderSize,Theme::flCardSpectatorBorderSize };
        vecOffset *= flCardScaling;

        pDrawList->AddRectFilled(vecPosition - vecOffset, vecPosition + vecSpecSize + vecOffset, ImColor(255, 0, 0), 8.f);
    }

    // Container
    pDrawList->AddRectFilled(vecPosition, vecPosition + vecSpecSize, Theme::colCardBackground, 8.f);

    // Draw Player Name
    pDrawList->AddText(Fonts::g_pDefaultSmallBold, flFontSize * 1.5f, vecPosition + ImVec2{ 16.f, 16.f }, Theme::colDefaultText, pSpec->m_szName);

    // Draw Team Name
    {
        ImVec2 vecTextMeasure = Fonts::g_pDefaultSmallBold->CalcTextSizeA(flFontSize * 1.7f, FLT_MAX, FLT_MAX, szTeamName);

        ImVec2 vecTextPos = vecPosition + ImVec2{ vecSpecSize.x - vecTextMeasure.x - 8.f, 16.f };

        pDrawList->AddText(Fonts::g_pDefaultSmallBold, flFontSize * 1.7f, vecTextPos, colTeamColor, szTeamName);
    }

    // Draw Health bar
    int nHealth = (int)pSpec->m_AnimationData.m_flRenderHealth;
    float flFill = pSpec->m_AnimationData.m_flRenderHealth / 100.f;
    float flFillWidth = vecSpecSize.x * flFill;
    ImVec2 vecFillStartRel = ImVec2{ 0.f, vecSpecSize.y - (24.f * flCardScaling) };
    ImVec2 vecFillStart = vecPosition + vecFillStartRel;
    ImVec2 vecFillEnd = vecFillStart + ImVec2{ flFillWidth, fabsf(vecFillStartRel.y - vecSpecSize.y) };

    if (nHealth < 0) {
        colTeamColor = ImColor(255, 0, 0);
        vecFillEnd = vecFillStart + ImVec2{ vecSpecSize.x, fabsf(vecFillStartRel.y - vecSpecSize.y) };
    }

    pDrawList->AddRectFilled(vecFillStart, vecFillEnd, colTeamColor, Theme::flCardRounding, ImDrawFlags_RoundCornersBottom);

    ImVec2 vecIconMeasurement = Fonts::g_pFontAwesome->CalcTextSizeA(flFontSize * 1.65f, FLT_MAX, FLT_MAX, ICON_FA_CROSSHAIRS);
    ImVec2 vecIconNegativeOffset = { vecIconMeasurement.x / 4.f, vecIconMeasurement.y / 3.f };

    if (nHealth > 0) {
        // Health icon
        pDrawList->AddText(Fonts::g_pFontAwesome, flFontSize * 1.65f, vecFillStart + ImVec2{ 12.f * flCardScaling, 1.f } - vecIconNegativeOffset, ImColor(255, 255, 255), ICON_FA_HEART);

        // Health text
        std::string szHealthText = std::to_string(nHealth);
        pDrawList->AddText(Fonts::g_pDefaultSmall, flFontSize, vecFillStart + ImVec2{ (32.f * flCardScaling), 0.f }, ImColor(255, 255, 255), szHealthText.c_str());

        Weapon* pDisplayWeapon = nullptr;
        if (pSpec->m_WeaponData.m_pPrimary)
            pDisplayWeapon = pSpec->m_WeaponData.m_pPrimary;
        else if (pSpec->m_WeaponData.m_pSecondary)
            pDisplayWeapon = pSpec->m_WeaponData.m_pSecondary;

        // Display that piece of shit
        if (pDisplayWeapon) {
            ImVec2 vecImageSize = { (float)pDisplayWeapon->m_Icon.m_uWidth, (float)pDisplayWeapon->m_Icon.m_uHeight };
            vecImageSize = ScaleToHeight(vecImageSize, fabsf(vecFillStartRel.y - vecSpecSize.y));

            ImVec2 vecImageMin = vecFillStart + ImVec2{ vecSpecSize.x - vecImageSize.x - 8.f, 0.f };
            ImVec2 vecImageMax = vecImageMin + vecImageSize;

            pDrawList->AddImage((ImTextureID)pDisplayWeapon->m_Icon.m_pTex, vecImageMin, vecImageMax);
        }

        // Money
        {
            ImVec2 vecTextPosition = vecPosition + ImVec2{ Theme::flCardInnerOffsetX, Theme::flCardInnerOffsetY + (flFontSize * 1.5f) + (24.f * flCardScaling) };
            std::string szMoney = "$" + std::to_string(pSpec->m_iMoney);
            pDrawList->AddText(Fonts::g_pDefaultSmall, flFontSize * 1.5f, vecTextPosition, Theme::colDefaultText, szMoney.c_str());
        }

        // Kill Count
        float flSLXOff = 0.f;
        if (pSpec->m_nKillsThisRound > 0) {
            ImVec2 vecTextPosition = vecPosition + ImVec2{ vecSpecSize.x - Theme::flCardInnerOffsetX, Theme::flCardInnerOffsetY + flFontSize + (24.f * flCardScaling) };
            std::string szKTR = std::to_string(pSpec->m_nKillsThisRound);
            pDrawList->AddText(Fonts::g_pDefaultSmall, flFontSize * 1.5f, vecTextPosition, Theme::colStatIcons, szKTR.c_str());

            vecTextPosition = vecPosition + ImVec2{ vecSpecSize.x - 24.f - Theme::flCardInnerOffsetX, flFontSize };
            pDrawList->AddText(Fonts::g_pFontAwesome, flFontSize * 1.65f, vecTextPosition, Theme::colStatIcons, ICON_FA_CROSSHAIRS);
            flSLXOff -= 24.f * flCardScaling;
        }

        // Utility
        for (size_t i = 0; i < pSpec->m_WeaponData.m_arrGrenades.size(); i++)
        {
            Weapon Weapon = pSpec->m_WeaponData.m_arrGrenades[i];

            ImVec2 vecUtilityIconSize = ImVec2{ (float)Weapon.m_Icon.m_uWidth, (float)Weapon.m_Icon.m_uHeight };
            vecUtilityIconSize *= flCardScaling;
            vecUtilityIconSize *= 0.85f;

            // Stupid offset not scaling correctly
            // FIXME! What's this piece of shit?
            float flOffset = 8.f;
            if (flCardScaling > 1.f)
                flOffset = 16.f * flCardScaling;

            ImVec2 vecUtilityIconMin = vecPosition + ImVec2{ vecSpecSize.x - Theme::flCardInnerOffsetX - flSLXOff - flOffset, Theme::flCardInnerOffsetY + (flFontSize * 1.5f) + (24.f * flCardScaling) };
            ImVec2 vecUtilityIconMax = vecUtilityIconMin + vecUtilityIconSize;

            pDrawList->AddImage((ImTextureID)Weapon.m_Icon.m_pTex, vecUtilityIconMin, vecUtilityIconMax, ImVec2{ 0.f, 0.f }, ImVec2{ 1.f, 1.f }, Theme::colUtilIcons);
            flSLXOff += vecUtilityIconSize.x;
        }

        if (pSpec->m_WeaponData.m_bHasDefuser) {
            IconData_t DefuserIcon = g_mWeaponIcons[FNV1A::HashConst("defuser")];
            ImVec2 vecUtilityIconSize = ImVec2{ (float)DefuserIcon.m_uWidth, (float)DefuserIcon.m_uHeight };
            vecUtilityIconSize *= flCardScaling;
            vecUtilityIconSize *= 1.f;

            float flOffset = 8.f;
            if (flCardScaling > 1.f)
                flOffset = 16.f * flCardScaling;

            // Why do i need to do this?
            flOffset += vecUtilityIconSize.x;

            ImVec2 vecUtilityIconMin = vecPosition + ImVec2{ vecSpecSize.x - Theme::flCardInnerOffsetX - flSLXOff - flOffset, Theme::flCardInnerOffsetY + (flFontSize * 1.5f) + (24.f * flCardScaling) };
            ImVec2 vecUtilityIconMax = vecUtilityIconMin + vecUtilityIconSize;

            pDrawList->AddImage((ImTextureID)DefuserIcon.m_pTex, vecUtilityIconMin, vecUtilityIconMax, ImVec2{ 0.f, 0.f }, ImVec2{ 1.f, 1.f }, Theme::colCounterTerroristsColor);
            flSLXOff += vecUtilityIconSize.x;
        }

        if (pSpec->m_WeaponData.m_bHasBomb) {
            IconData_t BombIcon = g_mWeaponIcons[FNV1A::HashConst("c4")];
            ImVec2 vecUtilityIconSize = ImVec2{ (float)BombIcon.m_uWidth, (float)BombIcon.m_uHeight };
            vecUtilityIconSize *= flCardScaling;
            vecUtilityIconSize *= 1.f;

            float flOffset = 8.f;
            if (flCardScaling > 1.f)
                flOffset = 16.f * flCardScaling;

            // But why?
            flOffset += vecUtilityIconSize.x;

            ImVec2 vecUtilityIconMin = vecPosition + ImVec2{ vecSpecSize.x - Theme::flCardInnerOffsetX - flSLXOff - flOffset, Theme::flCardInnerOffsetY + (flFontSize * 1.5f) + (24.f * flCardScaling) };
            ImVec2 vecUtilityIconMax = vecUtilityIconMin + vecUtilityIconSize;

            pDrawList->AddImage((ImTextureID)BombIcon.m_pTex, vecUtilityIconMin, vecUtilityIconMax, ImVec2{ 0.f, 0.f }, ImVec2{ 1.f, 1.f }, Theme::colTerroristsColor);
            flSLXOff += vecUtilityIconSize.x;
        }
    }
    else {
        std::string szDeathInfoText = std::to_string(pSpec->m_nDamageThisRound) + " DAMAGE THIS ROUND (" + std::to_string(pSpec->m_nKillsThisRound) + " KILLS)";
        pDrawList->AddText(Fonts::g_pDefaultSmallItalic, flFontSize, vecFillStart + ImVec2{ (12.f * flCardScaling), 0.f }, Theme::colDefaultText, szDeathInfoText.c_str());
    }
}
