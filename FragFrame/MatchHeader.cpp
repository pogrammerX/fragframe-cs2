#include "inc.h"

void DrawHeader(ImDrawList* pDrawList, const char* szTerrorTeamName, const char* szCounterTeamName) {
    // Draw Round info (score, roundtime, team names, etc)
    {
        ImVec2 vecHeaderSize = ImVec2{ 627.f, 84.f } *flCardScaling;
        ImVec2 vecHeaderPosition = ImVec2{
            (ImGui::GetIO().DisplaySize.x / 2.f) - (vecHeaderSize.x / 2.f),
            8.f
        };

        // Container
        pDrawList->AddRectFilled(vecHeaderPosition, vecHeaderPosition + vecHeaderSize, Theme::colCardBackground, Theme::flHeaderRounding);

        // Round timer
        {
            if (CSData::g_pGameRules->m_bBombPlanted()) {
                IconData_t Bomb = g_mWeaponIcons[FNV1A::HashConst("bomb_1024")];

                ImVec2 vecRealBombSize = ImVec2{ (float)Bomb.m_uWidth, (float)Bomb.m_uHeight };
                vecRealBombSize = ScaleToHeight(vecRealBombSize, Theme::flRoundTimeFontSize * flCardScaling);

                ImVec2 vecPos = vecHeaderPosition;
                vecPos += ImVec2{
                    (vecHeaderSize.x / 2.f) - (vecRealBombSize.x / 2.f),
                    10.f
                };

                pDrawList->AddImage((ImTextureID)Bomb.m_pTex, vecPos, vecPos + vecRealBombSize, ImVec2{ 0.f, 0.f }, ImVec2{ 1.f, 1.f }, ImColor{ 1.f, 0.f, 0.f, 1.f });
            }
            else {
                std::string szRoundTimerText = "";
                ImColor colRoundTimerColor = Theme::colDefaultText;
                if (CSData::g_pGameRules->m_bFreezePeriod()) {
                    colRoundTimerColor = ImColor(255, 0, 0);
                    int nRoundTimeLeft = CSData::g_pGameRules->GetFreezeTimeLeft();

                    nRoundTimeLeft = std::min(0, nRoundTimeLeft);

                    int nMinutes = nRoundTimeLeft / 60; // Calculate minutes
                    int nSeconds = nRoundTimeLeft % 60; // Calculate remaining seconds

                    // Format minutes and seconds as two digits
                    szRoundTimerText = (nMinutes <= 9 ? "0" : "") + std::to_string(nMinutes) + ":" +
                        (nSeconds <= 9 ? "0" : "") + std::to_string(nSeconds);
                }
                else {
                    int nRoundTimeLeft = CSData::g_pGameRules->GetRoundTimeLeft();

                    nRoundTimeLeft = std::min(0, nRoundTimeLeft);

                    int nMinutes = nRoundTimeLeft / 60; // Calculate minutes
                    int nSeconds = nRoundTimeLeft % 60; // Calculate remaining seconds

                    // Format minutes and seconds as two digits
                    szRoundTimerText = (nMinutes <= 9 ? "0" : "") + std::to_string(nMinutes) + ":" +
                        (nSeconds <= 9 ? "0" : "") + std::to_string(nSeconds);
                }

                ImVec2 vecTextSize = Fonts::g_pDefaultSmallBold->CalcTextSizeA(Theme::flRoundTimeFontSize * flCardScaling, FLT_MAX, FLT_MAX, szRoundTimerText.c_str());

                ImVec2 vecTextPos = vecHeaderPosition;
                vecTextPos += ImVec2{
                    (vecHeaderSize.x / 2.f) - (vecTextSize.x / 2.f),
                    10.f
                };

                pDrawList->AddText(Fonts::g_pDefaultSmallBold, Theme::flRoundTimeFontSize * flCardScaling, vecTextPos, colRoundTimerColor, szRoundTimerText.c_str());
            }
        }

        // Round number
        {
            std::string szRoundTimerText = "Round " + std::to_string(CSData::g_pGameRules->GetRound());

            ImVec2 vecTextSize = Fonts::g_pDefaultSmallBold->CalcTextSizeA(Theme::flRoundNumberFontSize * flCardScaling, FLT_MAX, FLT_MAX, szRoundTimerText.c_str());

            ImVec2 vecTextPos = vecHeaderPosition;
            vecTextPos += ImVec2{
                (vecHeaderSize.x / 2.f) - (vecTextSize.x / 2.f),
                10.f + (Theme::flRoundTimeFontSize * flCardScaling)
            };

            pDrawList->AddText(Fonts::g_pDefaultSmallBold, Theme::flRoundNumberFontSize * flCardScaling, vecTextPos, Theme::colDefaultText, szRoundTimerText.c_str());
        }

        ImColor colTransparent = ImColor(0, 0, 0, 0);
        // Left edge color
        // Default ImGui doesn't support multi color + rounded, so we gotta do it like this (and im too lazy to add it myself + this looks better anyway)
        {
            ImVec2 vecEdgeColorMin = vecHeaderPosition;
            ImVec2 vecEdgeColorMax = vecHeaderPosition + ImVec2{ 200.f * flCardScaling, vecHeaderSize.y };

            ImColor colEdgeColor = Theme::colTerroristsColor;

            // Fill a small rect on the Corner
            pDrawList->AddRectFilled(vecEdgeColorMin, vecEdgeColorMin + ImVec2{ 12.f, vecHeaderSize.y }, colEdgeColor, Theme::flHeaderRounding, ImDrawFlags_RoundCornersLeft);

            // Draw the gradient
            pDrawList->AddRectFilledMultiColor(vecEdgeColorMin + ImVec2{ 12.f, 0.f }, vecEdgeColorMax + ImVec2{ 12.f, 0.f }, colEdgeColor, colTransparent, colTransparent, colEdgeColor);
        }

        // Right edge color
        {
            ImVec2 vecEdgeColorMin = vecHeaderPosition + ImVec2{ vecHeaderSize.x - 200.f * flCardScaling - 12.f, 0.f };
            ImVec2 vecEdgeColorMax = vecHeaderPosition + ImVec2{ vecHeaderSize.x - 12.f, vecHeaderSize.y };

            ImColor colEdgeColor = Theme::colCounterTerroristsColor;

            // Fill a small rect on the Corner
            // Why 13? Shouldn't it be 12?
            pDrawList->AddRectFilled(vecEdgeColorMin + ImVec2{ 200.f * flCardScaling, 0.f }, vecEdgeColorMin + ImVec2{ 200.f * flCardScaling + 13.f, vecHeaderSize.y }, colEdgeColor, Theme::flHeaderRounding, ImDrawFlags_RoundCornersRight);

            // Draw the gradient
            pDrawList->AddRectFilledMultiColor(vecEdgeColorMin + ImVec2{ 1.f, 0.f }, vecEdgeColorMax + ImVec2{ 1.f, 0.f }, colTransparent, colEdgeColor, colEdgeColor, colTransparent);
        }

        float flMaxTeamNameWidth = vecHeaderSize.x * 0.3f;
        float flTeamNameFontSize = Theme::flTeamNameFontSize * std::max(1.f, flCardScaling * .75f);
        // Terror Team name (left)
        {
            float flScaledFontSize = CalculateScaledFontSize(Fonts::g_pDefaultSmallBold, szTerrorTeamName, flMaxTeamNameWidth, flTeamNameFontSize);
            float flXOffset = 0.f;

            ImVec2 vecTextSize = Fonts::g_pDefaultSmallBold->CalcTextSizeA(flScaledFontSize, FLT_MAX, FLT_MAX, szTerrorTeamName);
            flXOffset = fabsf((vecTextSize.x / 2.f) - (flMaxTeamNameWidth / 2.f));

            pDrawList->AddText(Fonts::g_pDefaultSmallBold, flScaledFontSize,
                ImVec2{
                    vecHeaderPosition.x + 8.f + flXOffset,
                    vecHeaderPosition.y + ((vecHeaderSize.y / 2.f) - (flScaledFontSize / 2.f))
                },
                Theme::colDefaultText, szTerrorTeamName);
        }

        // Counter-Terror Team name (right)
        {
            float flScaledFontSize = CalculateScaledFontSize(Fonts::g_pDefaultSmallBold, szCounterTeamName, flMaxTeamNameWidth, flTeamNameFontSize);
            float flXOffset = 0.f;

            ImVec2 vecTextSize = Fonts::g_pDefaultSmallBold->CalcTextSizeA(flScaledFontSize, FLT_MAX, FLT_MAX, szCounterTeamName);
            flXOffset = fabsf((vecTextSize.x / 2.f) - (flMaxTeamNameWidth / 2.f));

            ImVec2 vecTextMeasure = Fonts::g_pDefaultSmallBold->CalcTextSizeA(flScaledFontSize, FLT_MAX, FLT_MAX, szCounterTeamName);

            pDrawList->AddText(Fonts::g_pDefaultSmallBold, flScaledFontSize,
                ImVec2{
                    vecHeaderPosition.x + (vecHeaderSize.x - vecTextMeasure.x) - 8.f - flXOffset,
                    vecHeaderPosition.y + ((vecHeaderSize.y / 2.f) - (flScaledFontSize / 2.f))
                },
                Theme::colDefaultText, szCounterTeamName);
        }

        // Terror Team score (left)
        {
            std::string szScoreText = std::to_string(CSData::g_pTTeam->m_iScore());

            float flFontSize = Theme::flScoreFontSize * flCardScaling;
            ImVec2 vecTextMeasure = Fonts::g_pDefaultSmallBold->CalcTextSizeA(flFontSize, FLT_MAX, FLT_MAX, szScoreText.c_str());

            ImVec2 vecPosition =
            {
                vecHeaderPosition.x + ((vecHeaderSize.x * 0.36f) - (vecTextMeasure.x / 2.f)),
                vecHeaderPosition.y + ((vecHeaderSize.y / 2.f) - (flFontSize / 2.f))
            };

            pDrawList->AddText(Fonts::g_pDefaultSmallBold, flFontSize, vecPosition, Theme::colTerroristsColor, szScoreText.c_str());
        }

        // Counter Terror Team score (left)
        {
            std::string szScoreText = std::to_string(CSData::g_pCTTeam->m_iScore());

            float flFontSize = Theme::flScoreFontSize * flCardScaling;
            ImVec2 vecTextMeasure = Fonts::g_pDefaultSmallBold->CalcTextSizeA(flFontSize, FLT_MAX, FLT_MAX, szScoreText.c_str());

            ImVec2 vecPosition =
            {
                vecHeaderPosition.x + vecHeaderSize.x - ((vecHeaderSize.x * 0.36f) + (vecTextMeasure.x / 2.f)),
                vecHeaderPosition.y + ((vecHeaderSize.y / 2.f) - (flFontSize / 2.f))
            };

            pDrawList->AddText(Fonts::g_pDefaultSmallBold, flFontSize, vecPosition, Theme::colCounterTerroristsColor, szScoreText.c_str());
        }
    }
}