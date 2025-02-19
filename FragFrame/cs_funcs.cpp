#include "inc.h"

std::string GetWeaponNameFromDefIndex(int nDefIndex) {
	if (nDefIndex == WEAPON_NONE)					return "None";
	if (nDefIndex == WEAPON_DESERT_EAGLE)			return "Desert Eagle";
	if (nDefIndex == WEAPON_DUAL_BERETTAS)			return "Dual berettas";
	if (nDefIndex == WEAPON_FIVE_SEVEN)				return "Five-Seven";
	if (nDefIndex == WEAPON_GLOCK_18)				return "Glock-18";
	if (nDefIndex == WEAPON_AK_47)					return "AK-47";
	if (nDefIndex == WEAPON_AUG)					return "AUG";
	if (nDefIndex == WEAPON_AWP)					return "AWP";
	if (nDefIndex == WEAPON_FAMAS)					return "Famas";
	if (nDefIndex == WEAPON_G3SG1)					return "G3SG1";
	if (nDefIndex == WEAPON_GALIL_AR)				return "Galil AR";
	if (nDefIndex == WEAPON_M249)					return "M249";
	if (nDefIndex == WEAPON_M4A4)					return "M4A4";
	if (nDefIndex == WEAPON_MAC_10)					return "MAC-10";
	if (nDefIndex == WEAPON_P90)					return "P90";
	if (nDefIndex == WEAPON_MP5_SD)					return "MP5-SD";
	if (nDefIndex == WEAPON_UMP_45)					return "UMP-45";
	if (nDefIndex == WEAPON_XM1014)					return "Xm1014";
	if (nDefIndex == WEAPON_PP_BIZON)				return "PP-Bizon";
	if (nDefIndex == WEAPON_MAG_7)					return "MAG-7";
	if (nDefIndex == WEAPON_NEGEV)					return "Negev";
	if (nDefIndex == WEAPON_SAWED_OFF)				return "Sawed off";
	if (nDefIndex == WEAPON_TEC_9)					return "Tec9";
	if (nDefIndex == WEAPON_ZEUS_X27)				return "Zeus";
	if (nDefIndex == WEAPON_P2000)					return "P2000";
	if (nDefIndex == WEAPON_MP7)					return "MP7";
	if (nDefIndex == WEAPON_MP9)					return "MP9";
	if (nDefIndex == WEAPON_NOVA)					return "Nova";
	if (nDefIndex == WEAPON_P250)					return "P250";
	if (nDefIndex == WEAPON_RIOT_SHIELD)			return "Riot shield";
	if (nDefIndex == WEAPON_SCAR_20)				return "Scar 20";
	if (nDefIndex == WEAPON_SG_553)					return "SG 553";
	if (nDefIndex == WEAPON_SSG_08)					return "SSG";
	if (nDefIndex == WEAPON_KNIFE_T)				return "Knife";
	if (nDefIndex == WEAPON_KNIFE_CT)				return "Knife";
	if (nDefIndex == WEAPON_FLASHBANG)				return "Flashbang";
	if (nDefIndex == WEAPON_HIGH_EXPLOSIVE_GRENADE) return "HE Grenade";
	if (nDefIndex == WEAPON_SMOKE_GRENADE)			return "Smoke grenade";
	if (nDefIndex == WEAPON_MOLOTOV)				return "Molotov";
	if (nDefIndex == WEAPON_DECOY_GRENADE)			return "Decoy grenade";
	if (nDefIndex == WEAPON_INCENDIARY_GRENADE)		return "Incendiary grenade";
	if (nDefIndex == WEAPON_C4_EXPLOSIVE)			return "Bomb";
	if (nDefIndex == WEAPON_KEVLAR_VEST)			return "Kevlar vest";
	if (nDefIndex == WEAPON_KEVLAR_and_HELMET)		return "Kevlar and helmet";
	if (nDefIndex == WEAPON_HEAVY_ASSAULT_SUIT)		return "Heavy assault suit";
	if (nDefIndex == WEAPON_NO_LOCALIZED_NAME0)		return "No localized name";
	if (nDefIndex == WEAPON_DEFUSE_KIT)				return "Defuse kit";
	if (nDefIndex == WEAPON_RESCUE_KIT)				return "Rescue kit";
	if (nDefIndex == WEAPON_MEDI_SHOT)				return "Medi shot";
	if (nDefIndex == WEAPON_MUSIC_KIT)				return "Music kit";
	if (nDefIndex == WEAPON_KNIFE_GG)				return "Knife";
	if (nDefIndex == WEAPON_M4A1_S)					return "M4A1-S";
	if (nDefIndex == WEAPON_USP_S)					return "USP-S";
	if (nDefIndex == WEAPON_TRADE_UP_CONTRACT)		return "Trade up contract";
	if (nDefIndex == WEAPON_CZ75_AUTO)				return "CZ75 auto";
	if (nDefIndex == WEAPON_R8_REVOLVER)			return "R8 Revolver";
	if (nDefIndex == WEAPON_TACTICAL_AWARENESS_GRENADE) return "Tactical awareness grenade";
	if (nDefIndex == WEAPON_KNIFE3)					return "Knife";
	if (nDefIndex == WEAPON_AXE)					return "Axe";
	if (nDefIndex == WEAPON_HAMMER)					return "Hammer";
	if (nDefIndex == WEAPON_SPECTRAL_SHIV)			return "Spectral shiv";
	if (nDefIndex == WEAPON_FIRE_BOMB)				return "Fire bomb";
	if (nDefIndex == WEAPON_DIVERSION_DEVICE)		return "Diversion device";
	if (nDefIndex == WEAPON_FRAG_GRENADE)			return "Frag grenade";
	if (nDefIndex == WEAPON_SNOWBALL)				return "Snowball";
	if (nDefIndex == WEAPON_BUMP_MINE)				return "Bump mine";
	if (nDefIndex == WEAPON_BAYONET)				return "Bayonet";
	if (nDefIndex == WEAPON_CLASSIC_KNIFE)			return "Classic knife";
	if (nDefIndex == WEAPON_FLIP_KNIFE)				return "Flip knife";
	if (nDefIndex == WEAPON_GUT_KNIFE)				return "Gut knife";
	if (nDefIndex == WEAPON_KARAMBIT)				return "Karambit";
	if (nDefIndex == WEAPON_M9_BAYONET)				return "M9 Bayonet";
	if (nDefIndex == WEAPON_HUNTSMAN_KNIFE)			return "Huntsman Knife";
	if (nDefIndex == WEAPON_FALCHION_KNIFE)			return "Falchion Knife";
	if (nDefIndex == WEAPON_BOWIE_KNIFE)			return "Bowie Knife";
	if (nDefIndex == WEAPON_BUTTERFLY_KNIFE)		return "Butterfly Knife";
	if (nDefIndex == WEAPON_SHADOW_DAGGERS)			return "Shadow Daggers";
	if (nDefIndex == WEAPON_PARACORD_KNIFE)			return "Paracord Knife";
	if (nDefIndex == WEAPON_SURVIVAL_KNIFE)			return "Survival Knife";
	if (nDefIndex == WEAPON_URSUS_KNIFE)			return "Ursus Knife";
	if (nDefIndex == WEAPON_NAVAJA_KNIFE)			return "Navaja Knife";
	if (nDefIndex == WEAPON_NOMAD_KNIFE)			return "Nomad Knife";
	if (nDefIndex == WEAPON_STILETTO_KNIFE)			return "Stiletto Knife";
	if (nDefIndex == WEAPON_TALON_KNIFE)			return "Talon Knife";
	if (nDefIndex == WEAPON_SKELETON_KNIFE)			return "Skeleton Knife";
	return "Unknown";
}

std::string GetWeaponIdFromDefIndex(int nDefIndex) {
	if (nDefIndex == WEAPON_DESERT_EAGLE) return "deagle";
	if (nDefIndex == WEAPON_DUAL_BERETTAS) return "elite";
	if (nDefIndex == WEAPON_FIVE_SEVEN) return "fiveseven";
	if (nDefIndex == WEAPON_GLOCK_18) return "glock";
	if (nDefIndex == WEAPON_AK_47) return "ak47";
	if (nDefIndex == WEAPON_AUG) return "aug";
	if (nDefIndex == WEAPON_AWP) return "awp";
	if (nDefIndex == WEAPON_FAMAS) return "famas";
	if (nDefIndex == WEAPON_G3SG1) return "g3sg1";
	if (nDefIndex == WEAPON_GALIL_AR) return "galilar";
	if (nDefIndex == WEAPON_M249) return "m249";
	if (nDefIndex == WEAPON_M4A4) return "m4a1";
	if (nDefIndex == WEAPON_MAC_10) return "mac10";
	if (nDefIndex == WEAPON_P90) return "p90";
	if (nDefIndex == WEAPON_MP5_SD) return "mp5sd";
	if (nDefIndex == WEAPON_UMP_45) return "ump45";
	if (nDefIndex == WEAPON_XM1014) return "xm1014";
	if (nDefIndex == WEAPON_PP_BIZON) return "bizon";
	if (nDefIndex == WEAPON_MAG_7) return "mag7";
	if (nDefIndex == WEAPON_NEGEV) return "negev";
	if (nDefIndex == WEAPON_SAWED_OFF) return "sawedoff";
	if (nDefIndex == WEAPON_TEC_9) return "tec9.svg";
	if (nDefIndex == WEAPON_ZEUS_X27) return "taser";
	if (nDefIndex == WEAPON_P2000) return "p2000";
	if (nDefIndex == WEAPON_MP7) return "mp7";
	if (nDefIndex == WEAPON_MP9) return "mp9";
	if (nDefIndex == WEAPON_NOVA) return "nova";
	if (nDefIndex == WEAPON_P250) return "p250";
	if (nDefIndex == WEAPON_RIOT_SHIELD) return "shield";
	if (nDefIndex == WEAPON_SCAR_20) return "scar20";
	if (nDefIndex == WEAPON_SG_553) return "sg556";
	if (nDefIndex == WEAPON_SSG_08) return "ssg08";
	if (nDefIndex == WEAPON_KNIFE_T) return "knife";
	if (nDefIndex == WEAPON_KNIFE_CT) return "knife";
	if (nDefIndex == WEAPON_FLASHBANG) return "flashbang";
	if (nDefIndex == WEAPON_HIGH_EXPLOSIVE_GRENADE) return "hegrenade";
	if (nDefIndex == WEAPON_SMOKE_GRENADE) return "smokegrenade";
	if (nDefIndex == WEAPON_MOLOTOV) return "molotov";
	if (nDefIndex == WEAPON_DECOY_GRENADE) return "decoy";
	if (nDefIndex == WEAPON_INCENDIARY_GRENADE) return "incgrenade";
	if (nDefIndex == WEAPON_C4_EXPLOSIVE) return "c4";
	if (nDefIndex == WEAPON_MEDI_SHOT) return "healthshot";;
	if (nDefIndex == WEAPON_KNIFE_GG) "knife";
	if (nDefIndex == WEAPON_M4A1_S) "m4a1_silencer";
	if (nDefIndex == WEAPON_USP_S) return "usp_silencer";
	if (nDefIndex == WEAPON_CZ75_AUTO) return "cz75a";
	if (nDefIndex == WEAPON_R8_REVOLVER) return "revolver";
	if (nDefIndex == WEAPON_BREACH_CHARGE) return "breachcharge";
	if (nDefIndex == WEAPON_KNIFE3) return "knife";
	if (nDefIndex == WEAPON_AXE) return "axe";
	if (nDefIndex == WEAPON_HAMMER) return "hammer";
	if (nDefIndex == WEAPON_FIRE_BOMB) return "firebomb";
	if (nDefIndex == WEAPON_FRAG_GRENADE) return "frag_grenade";
	if (nDefIndex == WEAPON_SNOWBALL) return "snowball";
	if (nDefIndex == WEAPON_BUMP_MINE) return "bumpmine";
	return "";
}

int C_CSGameRules::GetFreezeTimeLeft() {
	if (!m_bFreezePeriod())
		return 0;

	return CSIData::m_nHudTeamCounterLabel;
}

int C_CSGameRules::GetRoundTimeLeft() {
	if (m_bFreezePeriod() || m_bWarmupPeriod())
		return -1;

	return CSIData::m_nHudTeamCounterLabel;
}

int C_CSGameRules::GetRound() {
	return m_totalRoundsPlayed() + 1;
}