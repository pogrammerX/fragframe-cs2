#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "FNV1A.h"

struct ImDrawList;
class ID3D11ShaderResourceView;

extern std::unordered_map<FNV1A_t, ID3D11ShaderResourceView*> g_mRadarImages;

// because we remove the default CS2 panorama, we have to render some things ourself
void DrawPanorama(ImDrawList* pDrawList);