#include "inc.h"

Weapon* LoadWeapon(C_CSWeaponBaseGun* pWeapon) {
    // Save some memory
    static std::unordered_map<int, Weapon*> mWeapons{};

    int nDefIndex = pWeapon->m_AttributeManager()->m_Item()->m_iItemDefinitionIndex();

    if (mWeapons.contains(nDefIndex))
        return mWeapons[nDefIndex];
    else {
        Weapon* pDataWeapon = new Weapon{};
        FNV1A_t uWeaponIdHash = FNV1A::Hash(GetWeaponIdFromDefIndex(nDefIndex).c_str());
        pDataWeapon->m_Icon = g_mWeaponIcons[uWeaponIdHash];
        pDataWeapon->m_nDefIndex = nDefIndex;
        pDataWeapon->m_szName = GetWeaponNameFromDefIndex(nDefIndex);
        mWeapons.insert({ nDefIndex, pDataWeapon });
        return pDataWeapon;
    }
}

std::string TransformTextToUpper(std::string szText) {
    std::string str = szText;
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}


ID3D11ShaderResourceView* CreateTexture(std::vector<uint8_t> Data, int nWidth, int nHeight) {
    D3D11_TEXTURE2D_DESC textureDesc;

    ZeroMemory(&textureDesc, sizeof(textureDesc));
    textureDesc.Width = nWidth;
    textureDesc.Height = nHeight;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // Assuming 8 bits per channel
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA initData;
    ZeroMemory(&initData, sizeof(initData));
    initData.pSysMem = Data.data();
    initData.SysMemPitch = nWidth * 4; // Assuming 4 bytes per pixel (R8G8B8A8_UNORM)

    ID3D11Texture2D* texture = nullptr;
    HRESULT hr = CSData::g_pDevice->CreateTexture2D(&textureDesc, &initData, &texture);
    if (FAILED(hr)) {
        return nullptr;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = textureDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    ID3D11ShaderResourceView* srv = nullptr;
    hr = CSData::g_pDevice->CreateShaderResourceView(texture, &srvDesc, &srv);
    if (FAILED(hr)) {
        texture->Release();
        return nullptr;
    }

    return srv;
}

float CalculateScaledFontSize(ImFont* font, const char* text, float maxWidth, float initialFontSize) {
    ImVec2 textSize = font->CalcTextSizeA(initialFontSize, FLT_MAX, FLT_MAX, text);
    if (textSize.x <= maxWidth) {
        return initialFontSize; // No scaling needed
    }
    return initialFontSize * (maxWidth / textSize.x); // Scale down the font size
}

void RenderSlidingColorRect(ImDrawList* drawList, ImVec2 topLeft, ImVec2 size, float rounding) {
    ImGuiIO& io = ImGui::GetIO();

    static float flColorShiftTime = 0.f;
    flColorShiftTime += io.DeltaTime;

    ImColor colors[4];
    for (int i = 0; i < 4; ++i) {
        colors[i] = ImColor::HSV(fmod(flColorShiftTime * 60.0f + i * 90.0f, 360.0f) / 360.0f, 1.0f, 1.0f);
    }

    drawList->AddRectFilledMultiColor(
        topLeft,
        ImVec2(topLeft.x + size.x, topLeft.y + size.y),
        colors[0],
        colors[1],
        colors[1],
        colors[0],
        rounding,
        ImDrawFlags_RoundCornersAll
    );
}


ImVec2 ScaleToHeight(const ImVec2& vecImageSize, float targetHeight) {
    // Calculate the scaling factor based on the target height
    float scaleFactor = targetHeight / vecImageSize.y;

    // Calculate the new width while maintaining the aspect ratio
    float newWidth = vecImageSize.x * scaleFactor;

    // Return the new size
    return ImVec2(newWidth, targetHeight);
}

PlayerData* GetPlayerDataFromController(CBasePlayerController* pController) {
    for (size_t i = 0; i < g_arrDataPlayers.size(); i++)
    {
        PlayerData* pData = g_arrDataPlayers[i];
        if (pData->m_hController == pController->m_hEntityHandle()) {
            return pData;
            break;
        }
    }
    return nullptr;
}

PlayerData* GetPlayerDataFromPawn(C_CSPlayerPawn* pPawn) {
    for (size_t i = 0; i < g_arrDataPlayers.size(); i++)
    {
        PlayerData* pData = g_arrDataPlayers[i];
        if (pData->m_hPawn == pPawn->m_hEntityHandle()) {
            return pData;
            break;
        }
    }
    return nullptr;
}