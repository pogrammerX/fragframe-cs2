#include "inc.h"
#include "3rdparty/imgui/imgui.h"

void CAnimation::Tick(float flDeltaTime) {
	if (flDeltaTime == 0.f) // if flDeltaTime is 0, we should just use ImGui's delta time, to make using this function less annoying
		flDeltaTime = ImGui::GetIO().DeltaTime;

	m_flProgress += flDeltaTime * m_flSpeed;
    m_flProgress = clmp(m_flProgress, 0.f, 1.f);
}

float CAnimation::GetValue() {
    // fucky math shit i wont explain
    if (m_flStart < m_flEnd) {
        float flDifference = m_flEnd - m_flStart;
        float flAmount = m_flProgress * flDifference;
        return flAmount + m_flStart;
    }
    else {
        float flDifference = m_flStart - m_flEnd;
        float flAmount = m_flProgress * flDifference;
        return m_flStart - flAmount;  // subtract flAmount from m_flStart
    }
}