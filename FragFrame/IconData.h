#pragma once
#include <cstdint>

class ID3D11ShaderResourceView;
struct IconData_t {
	ID3D11ShaderResourceView* m_pTex;
	uint32_t m_uWidth;
	uint32_t m_uHeight;
};