#include "inc.h"
#include "3rdparty/imgui/imgui_internal.h" // Stupid ImGui defining ImRect in the internal header...

float DrawPlayerCard(PlayerData* pPlayer, float flDeltaTime, ImDrawList* pDrawList, float flOffsetY, bool bSpec) {
    // Math stuff
    ImVec2 vecCardPosition = { Theme::flCardOffset, flOffsetY };
    ImColor colTeamColor = Theme::colTerroristsColor;
    float flFontSize = Theme::flCardFontSize * flCardScaling;
    float flPlayerCardWidth = g_flPlayerCardWidth * flCardScaling;
    float flPlayerCardHeight = g_flPlayerCardHeight * flCardScaling;

    // CT overrides
    if (pPlayer->m_nTeam == 1) {
        vecCardPosition = { ImGui::GetIO().DisplaySize.x - flPlayerCardWidth - Theme::flCardOffset, flOffsetY };
        colTeamColor = Theme::colCounterTerroristsColor;
    }

    if (bSpec) {
        ImVec2 vecOffset = { Theme::flCardSpectatorBorderSize, Theme::flCardSpectatorBorderSize };
        vecOffset *= flCardScaling;

        RenderSlidingColorRect(pDrawList, vecCardPosition - vecOffset, ImVec2{ flPlayerCardWidth, flPlayerCardHeight } + (vecOffset * 2.f), Theme::flCardRounding);
    }

    // Container
    pDrawList->AddRectFilled(vecCardPosition, vecCardPosition + ImVec2{ flPlayerCardWidth, flPlayerCardHeight }, Theme::colCardBackground, Theme::flCardRounding);

    ImVec2 vecIconMeasurement = Fonts::g_pFontAwesome->CalcTextSizeA(flFontSize * 1.65f, FLT_MAX, FLT_MAX, ICON_FA_CROSSHAIRS);
    ImVec2 vecIconNegativeOffset = { vecIconMeasurement.x / 4.f, vecIconMeasurement.y / 3.f };

    // Clickability
    if (Theme::bAllowMouse && ImGui::GetIO().MouseClicked[0]) {
        ImRect rCardRect{
            vecCardPosition,
            vecCardPosition + ImVec2{ flPlayerCardWidth, flPlayerCardHeight }
        };

        if (rCardRect.Contains(ImGui::GetIO().MousePos)) {
            if (!CSData::g_pTargetSpectatorOverride)
                CSData::g_pTargetSpectatorOverride = (CBaseHandle*)malloc(sizeof(CBaseHandle));
            *CSData::g_pTargetSpectatorOverride = pPlayer->m_hPawn;

            C_CSPlayerPawn* pLocalPawn = CSData::g_pGameEntitySystem->Get<C_CSPlayerPawn>(CSData::g_pLocalController->m_hPawn());
            if (pLocalPawn) {
                CPlayer_ObserverServices* pObserverServices = pLocalPawn->m_pObserverServices();

                if (pObserverServices) {
                    // Modifying schema -- Oh noes!
                    pObserverServices->m_iObserverMode() = ObserverMode_t::OBS_MODE_IN_EYE;
                    pObserverServices->m_iObserverLastMode() = ObserverMode_t::OBS_MODE_IN_EYE;
                    pObserverServices->m_bForcedObserverMode() = true;
                }
            }
        }
    }

    // Health animation
    {
        // Set target health, if it's outdated
        if (pPlayer->m_AnimationData.m_flRenderHealth != pPlayer->m_iHealth && pPlayer->m_AnimationData.m_flTargetHealth != (float)pPlayer->m_iHealth)
            pPlayer->m_AnimationData.m_flTargetHealth = (float)pPlayer->m_iHealth;

        // If the difference is higher than .15f, do the animation
        if (fabsf(pPlayer->m_AnimationData.m_flRenderHealth - pPlayer->m_AnimationData.m_flTargetHealth) >= .15f) {
            // Calculate the direction the animation needs to go
            float flDirection = pPlayer->m_AnimationData.m_flTargetHealth - (float)pPlayer->m_AnimationData.m_flRenderHealth;

            // Adjust the direction by delta time and animation speed
            flDirection *= flDeltaTime * Theme::flGlobalAnimationSpeed;

            // Add the direction to the render health
            pPlayer->m_AnimationData.m_flRenderHealth += flDirection;
        }
        else
            pPlayer->m_AnimationData.m_flRenderHealth = pPlayer->m_AnimationData.m_flTargetHealth; // else just set render health to the target health
    }

    // What's the point if the player is dead?
    if ((int)pPlayer->m_AnimationData.m_flRenderHealth > 0) {
        // First line (player name, kill count, death count)
        {
            // Player name
            {
                ImVec2 vecTextPosition = vecCardPosition + ImVec2{ Theme::flCardInnerOffsetX, Theme::flCardInnerOffsetY };
                pDrawList->AddText(Fonts::g_pDefaultSmall, flFontSize, vecTextPosition, Theme::colDefaultText, pPlayer->m_szName);
            }

            float flStatsOffset = Theme::flCardInnerOffsetX * flCardScaling; // Start from the right side

            // Deaths
            {
                // Text
                ImVec2 vecTextPosition = vecCardPosition + ImVec2{ flPlayerCardWidth - flStatsOffset, Theme::flCardInnerOffsetY };
                std::string szDeathsText = std::to_string(pPlayer->m_iDeaths);
                pDrawList->AddText(Fonts::g_pDefaultSmall, flFontSize, vecTextPosition, colTeamColor, szDeathsText.c_str());
                flStatsOffset += 18.f * flCardScaling; // Move the offset leftwards for the icon

                // Icon, for some reason we have to do negative offsets because its offset and too small
                // Fuck you font awesome
                vecTextPosition = vecCardPosition + ImVec2{ flPlayerCardWidth - flStatsOffset, Theme::flCardInnerOffsetY } - vecIconNegativeOffset;
                pDrawList->AddText(Fonts::g_pFontAwesome, flFontSize * 1.65f, vecTextPosition, Theme::colStatIcons, ICON_FA_SKULL);
                flStatsOffset += 48.f * flCardScaling; // Move the offset leftwards for the next element (if any)
            }

            // Kills
            {
                // Text
                ImVec2 vecTextPosition = vecCardPosition + ImVec2{ flPlayerCardWidth - flStatsOffset, Theme::flCardInnerOffsetY };
                std::string szKillsText = std::to_string(pPlayer->m_iKills);
                pDrawList->AddText(Fonts::g_pDefaultSmall, flFontSize, vecTextPosition, colTeamColor, szKillsText.c_str());
                flStatsOffset += 18.f * flCardScaling; // Move the offset leftwards for the icon

                // Icon, again no idea whats up with FontAwesome
                vecTextPosition = vecCardPosition + ImVec2{ flPlayerCardWidth - flStatsOffset, Theme::flCardInnerOffsetY } - vecIconNegativeOffset;
                pDrawList->AddText(Fonts::g_pFontAwesome, flFontSize * 1.65f, vecTextPosition, Theme::colStatIcons, ICON_FA_CROSSHAIRS);
                flStatsOffset += 48.f * flCardScaling; // Move the offset leftwards for the next element (if any)
            }
        }

        // Second line (money, kills this round, utility)
        {
            // Money
            {
                ImVec2 vecTextPosition = vecCardPosition + ImVec2{ Theme::flCardInnerOffsetX, Theme::flCardInnerOffsetY + flFontSize + 6.f };
                std::string szMoney = "$" + std::to_string(pPlayer->m_iMoney);
                pDrawList->AddText(Fonts::g_pDefaultSmall, flFontSize, vecTextPosition, Theme::colDefaultText, szMoney.c_str());
            }

            // Kill Count
            float flSLXOff = 0.f;
            if (pPlayer->m_nKillsThisRound > 0) {
                ImVec2 vecTextPosition = vecCardPosition + ImVec2{ flPlayerCardWidth - Theme::flCardInnerOffsetX, Theme::flCardInnerOffsetY + flFontSize + 6.f };
                std::string szKTR = std::to_string(pPlayer->m_nKillsThisRound);
                pDrawList->AddText(Fonts::g_pDefaultSmall, flFontSize, vecTextPosition, Theme::colStatIcons, szKTR.c_str());

                vecTextPosition = vecCardPosition + ImVec2{ flPlayerCardWidth - 24.f - Theme::flCardInnerOffsetX, flFontSize };
                pDrawList->AddText(Fonts::g_pFontAwesome, flFontSize * 1.65f, vecTextPosition, Theme::colStatIcons, ICON_FA_CROSSHAIRS);
                flSLXOff -= 24.f * flCardScaling;
            }

            // Utility
            for (size_t i = 0; i < pPlayer->m_WeaponData.m_arrGrenades.size(); i++)
            {
                Weapon Weapon = pPlayer->m_WeaponData.m_arrGrenades[i];

                ImVec2 vecUtilityIconSize = ImVec2{ (float)Weapon.m_Icon.m_uWidth, (float)Weapon.m_Icon.m_uHeight };
                vecUtilityIconSize *= flCardScaling;
                vecUtilityIconSize *= 0.85f;

                // Stupid offset not scaling correctly
                // FIXME! What's this piece of shit?
                float flOffset = 8.f;
                if (flCardScaling > 1.f)
                    flOffset = 16.f * flCardScaling;

                ImVec2 vecUtilityIconMin = vecCardPosition + ImVec2{ flPlayerCardWidth - Theme::flCardInnerOffsetX - flSLXOff - flOffset, Theme::flCardInnerOffsetY + flFontSize + 2.f };
                ImVec2 vecUtilityIconMax = vecUtilityIconMin + vecUtilityIconSize;

                pDrawList->AddImage((ImTextureID)Weapon.m_Icon.m_pTex, vecUtilityIconMin, vecUtilityIconMax, ImVec2{ 0.f, 0.f }, ImVec2{ 1.f, 1.f }, Theme::colUtilIcons);
                flSLXOff += vecUtilityIconSize.x;
            }

            if (pPlayer->m_WeaponData.m_bHasDefuser) {
                IconData_t DefuserIcon = g_mWeaponIcons[FNV1A::HashConst("defuser")];
                ImVec2 vecUtilityIconSize = ImVec2{ (float)DefuserIcon.m_uWidth, (float)DefuserIcon.m_uHeight };
                vecUtilityIconSize *= flCardScaling;
                vecUtilityIconSize *= 0.85f;

                float flOffset = 8.f;
                if (flCardScaling > 1.f)
                    flOffset = 16.f * flCardScaling;

                // Why do i need to do this?
                flOffset += vecUtilityIconSize.x;

                ImVec2 vecUtilityIconMin = vecCardPosition + ImVec2{ flPlayerCardWidth - Theme::flCardInnerOffsetX - flSLXOff - flOffset, Theme::flCardInnerOffsetY + flFontSize + 2.f };
                ImVec2 vecUtilityIconMax = vecUtilityIconMin + vecUtilityIconSize;

                pDrawList->AddImage((ImTextureID)DefuserIcon.m_pTex, vecUtilityIconMin, vecUtilityIconMax, ImVec2{ 0.f, 0.f }, ImVec2{ 1.f, 1.f }, Theme::colCounterTerroristsColor);
                flSLXOff += vecUtilityIconSize.x;
            }

            if (pPlayer->m_WeaponData.m_bHasBomb) {
                IconData_t BombIcon = g_mWeaponIcons[FNV1A::HashConst("c4")];
                ImVec2 vecUtilityIconSize = ImVec2{ (float)BombIcon.m_uWidth, (float)BombIcon.m_uHeight };
                vecUtilityIconSize *= flCardScaling;
                vecUtilityIconSize *= 0.85f;

                float flOffset = 8.f;
                if (flCardScaling > 1.f)
                    flOffset = 16.f * flCardScaling;

                // But why?
                flOffset += vecUtilityIconSize.x;

                ImVec2 vecUtilityIconMin = vecCardPosition + ImVec2{ flPlayerCardWidth - Theme::flCardInnerOffsetX - flSLXOff - flOffset, Theme::flCardInnerOffsetY + flFontSize + 2.f };
                ImVec2 vecUtilityIconMax = vecUtilityIconMin + vecUtilityIconSize;

                pDrawList->AddImage((ImTextureID)BombIcon.m_pTex, vecUtilityIconMin, vecUtilityIconMax, ImVec2{ 0.f, 0.f }, ImVec2{ 1.f, 1.f }, Theme::colTerroristsColor);
                flSLXOff += vecUtilityIconSize.x;
            }
        }

        // Third line (Health + Armor, Primary / Secondary weapon)
        {
            // Health bar
            int nHealth = (int)pPlayer->m_AnimationData.m_flRenderHealth;
            float flFill = pPlayer->m_AnimationData.m_flRenderHealth / 100.f;
            float flFillWidth = flPlayerCardWidth * flFill;
            ImVec2 vecFillStartRel = ImVec2{ 0.f, flPlayerCardHeight - (24.f * flCardScaling) };
            ImVec2 vecFillStart = vecCardPosition + vecFillStartRel;
            ImVec2 vecFillEnd = vecFillStart + ImVec2{ flFillWidth, fabsf(vecFillStartRel.y - flPlayerCardHeight) };
            pDrawList->AddRectFilled(vecFillStart, vecFillEnd, colTeamColor, Theme::flCardRounding, ImDrawFlags_RoundCornersBottom);

            // Health icon
            pDrawList->AddText(Fonts::g_pFontAwesome, flFontSize * 1.65f, vecFillStart + ImVec2{ 12.f * flCardScaling, 1.f } - vecIconNegativeOffset, ImColor(255, 255, 255), ICON_FA_HEART);

            // Health text
            std::string szHealthText = std::to_string(nHealth);
            pDrawList->AddText(Fonts::g_pDefaultSmall, flFontSize, vecFillStart + ImVec2{ (32.f * flCardScaling), 0.f }, ImColor(255, 255, 255), szHealthText.c_str());

            // Weapon
            Weapon* pDisplayWeapon = nullptr;
            if (pPlayer->m_WeaponData.m_pPrimary)
                pDisplayWeapon = pPlayer->m_WeaponData.m_pPrimary;
            else if (pPlayer->m_WeaponData.m_pSecondary)
                pDisplayWeapon = pPlayer->m_WeaponData.m_pSecondary;

            // Display that piece of shit
            if (pDisplayWeapon) {
                ImVec2 vecImageSize = { (float)pDisplayWeapon->m_Icon.m_uWidth, (float)pDisplayWeapon->m_Icon.m_uHeight };
                vecImageSize = ScaleToHeight(vecImageSize, fabsf(vecFillStartRel.y - flPlayerCardHeight));

                ImVec2 vecImageMin = vecFillStart + ImVec2{ flPlayerCardWidth - vecImageSize.x - 8.f, 0.f };
                ImVec2 vecImageMax = vecImageMin + vecImageSize;

                // do we really want this here?
                // TODO: make this a theme variable
                // 
                //if (pPlayer->m_nTeam == 1) {
                //    // Swap those X coords to flip the image
                //    float flX1 = vecImageMin.x;
                //    vecImageMin.x = vecImageMax.x;
                //    vecImageMax.x = flX1;
                //}

                pDrawList->AddImage((ImTextureID)pDisplayWeapon->m_Icon.m_pTex, vecImageMin, vecImageMax);
            }
        }
    }
    else {
        // Simplified version of what's above

        // First line (player name, kill count, death count)
        {
            // Player name
            {
                ImVec2 vecTextPosition = vecCardPosition + ImVec2{ Theme::flCardInnerOffsetX, Theme::flCardInnerOffsetY };
                pDrawList->AddText(Fonts::g_pDefaultSmall, flFontSize, vecTextPosition, Theme::colDefaultText, pPlayer->m_szName);
            }

            float flStatsOffset = Theme::flCardInnerOffsetX * flCardScaling; // Start from the right side

            // Deaths
            {
                // Text
                ImVec2 vecTextPosition = vecCardPosition + ImVec2{ flPlayerCardWidth - flStatsOffset, Theme::flCardInnerOffsetY };
                std::string szDeathsText = std::to_string(pPlayer->m_iDeaths);
                pDrawList->AddText(Fonts::g_pDefaultSmall, flFontSize, vecTextPosition, colTeamColor, szDeathsText.c_str());
                flStatsOffset += 18.f * flCardScaling; // Move the offset leftwards for the icon

                // Icon, for some reason we have to do negative offsets because its offset and too small
                // Fuck you font awesome
                vecTextPosition = vecCardPosition + ImVec2{ flPlayerCardWidth - flStatsOffset, Theme::flCardInnerOffsetY } - vecIconNegativeOffset;
                pDrawList->AddText(Fonts::g_pFontAwesome, flFontSize * 1.65f, vecTextPosition, Theme::colStatIcons, ICON_FA_SKULL);
                flStatsOffset += 48.f * flCardScaling; // Move the offset leftwards for the next element (if any)
            }

            // Kills
            {
                // Text
                ImVec2 vecTextPosition = vecCardPosition + ImVec2{ flPlayerCardWidth - flStatsOffset, Theme::flCardInnerOffsetY };
                std::string szKillsText = std::to_string(pPlayer->m_iKills);
                pDrawList->AddText(Fonts::g_pDefaultSmall, flFontSize, vecTextPosition, colTeamColor, szKillsText.c_str());
                flStatsOffset += 18.f * flCardScaling; // Move the offset leftwards for the icon

                // Icon, again no idea whats up with FontAwesome
                vecTextPosition = vecCardPosition + ImVec2{ flPlayerCardWidth - flStatsOffset, Theme::flCardInnerOffsetY } - vecIconNegativeOffset;
                pDrawList->AddText(Fonts::g_pFontAwesome, flFontSize * 1.65f, vecTextPosition, Theme::colStatIcons, ICON_FA_CROSSHAIRS);
                flStatsOffset += 48.f * flCardScaling; // Move the offset leftwards for the next element (if any)
            }
        }

        // Second line (money)
        {
            // Money
            {
                ImVec2 vecTextPosition = vecCardPosition + ImVec2{ Theme::flCardInnerOffsetX, Theme::flCardInnerOffsetY + flFontSize + 6.f };
                std::string szMoney = "$" + std::to_string(pPlayer->m_iMoney);
                pDrawList->AddText(Fonts::g_pDefaultSmall, flFontSize, vecTextPosition, Theme::colDefaultText, szMoney.c_str());
            }
        }

        // Third line (death info)
        {
            ImVec2 vecFillStartRel = ImVec2{ 0.f, flPlayerCardHeight - (24.f * flCardScaling) };
            ImVec2 vecFillStart = vecCardPosition + vecFillStartRel;
            ImVec2 vecFillEnd = vecFillStart + ImVec2{ flPlayerCardWidth, fabsf(vecFillStartRel.y - flPlayerCardHeight) };
            pDrawList->AddRectFilled(vecFillStart, vecFillEnd, ImColor(255, 0, 0), Theme::flCardRounding, ImDrawFlags_RoundCornersBottom);

            std::string szDeathInfoText = std::to_string(pPlayer->m_nDamageThisRound) + " DAMAGE THIS ROUND (" + std::to_string(pPlayer->m_nKillsThisRound) + " KILLS)";
            pDrawList->AddText(Fonts::g_pDefaultSmallItalic, flFontSize, vecFillStart + ImVec2{ (12.f * flCardScaling), 0.f }, Theme::colDefaultText, szDeathInfoText.c_str());
        }
    }

    return flPlayerCardHeight + (6.f * flCardScaling);
}
