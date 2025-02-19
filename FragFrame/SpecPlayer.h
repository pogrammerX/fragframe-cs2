#pragma once

struct ImDrawList;
struct PlayerData;
struct ImColor;
void DrawSpecPlayer(ImDrawList* pDrawList, PlayerData* pSpec, const char* szTeamName, ImColor colTeamColor);