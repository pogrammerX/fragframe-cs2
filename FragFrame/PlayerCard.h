#pragma once
#include <string>
#include <vector>
#include "IconData.h"
#include "Handle.h"

struct Weapon {
    IconData_t m_Icon;
    std::string m_szName;
    int m_nDefIndex;
};

struct PlayerWeaponData {
    Weapon* m_pPrimary;
    Weapon* m_pSecondary;
    std::vector<Weapon> m_arrGrenades;
    bool m_bHasArmor;
    bool m_bHasHelmet;

    bool m_bHasDefuser;
    bool m_bHasBomb;
};

struct AnimationData {
    float m_flRenderHealth;
    float m_flTargetHealth;

};

struct PlayerData {
    const char* m_szName;
    int m_iHealth;
    int m_iKills;
    int m_iDeaths;
    int m_iMoney;
    int m_nTeam;
    int m_nDamageThisRound;
    int m_nKillsThisRound;

    CBaseHandle m_hController;
    CBaseHandle m_hPawn;

    PlayerWeaponData m_WeaponData;
    AnimationData m_AnimationData;
};

extern float flCardScaling;

// We don't allowing theming on this, as we scale it with flCardScaling and don't want to fuck up our layout
const float g_flPlayerCardWidth = 272.f;
const float g_flPlayerCardHeight = 86.f;

struct ImDrawList;
float DrawPlayerCard(PlayerData* pPlayer, float flDeltaTime, ImDrawList* pDrawList, float flOffsetY, bool bSpec);