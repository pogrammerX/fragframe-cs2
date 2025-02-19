/*

	FragFrame - Customizable Counter-Strike 2 Overlay for Tournaments and LAN Events

	Developed by pogrammerX (https://github.com/pogrammerX)

*/

// Purpose: one header to include everything we need, cuz im lazy and dont like include conflicts :D
#pragma once

#define FF_VERSION "1.0" // Version string for FragFrame
#define FF_PATH "C:\\Program Files\\FragFrame\\" // FragFrame base path

// Some predefined colors
#define CS_CLR_RED		Color(1.f, 0.f, 0.f)
#define CS_CLR_GREEN	Color(0.f, 1.f, 0.f)
#define CS_CLR_WHITE	Color(1.f, 1.f, 1.f)
#define CS_CLR_BLUE		Color(0.f, 0.f, 1.f)

#include <Windows.h>
#include <string>
#include "pe64.h"
#include "memory.h"
#include <unordered_map>
#include <algorithm>
#include <dxgi.h>
#include <d3d11.h>
#include "FNV1A.h"
#include "Color.h"
#include "vector.h"
#include "qangle.h"
#include "ischemasystem.h"
#include "Animation.h"
#include "Data.h"
#include "cs_classes.h"
#include "const.h"
#include "Handle.h"
#include "gameentitysystem.h"
#include "Hooks.h"
#include "Util.h"
#include "FontAwesome.h"
#include "State.h"
#include "Interfaces.h"
#include "IconData.h"
#include "PlayerCard.h"
#include "SpecPlayer.h"
#include "MatchHeader.h"