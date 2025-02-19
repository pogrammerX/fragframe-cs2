#pragma once

/// <summary>
/// Stores animation specific data
/// </summary>
class CAnimation {
public:
	float m_flSpeed;
	float m_flStart;
	float m_flEnd;
	float m_flProgress;
	bool m_bPlaying;

	CAnimation() {
		m_flStart = 0.f;
		m_flEnd = 1.f;
		m_flSpeed = 1.f;
		m_flProgress = 0;
	}

	void Tick(float flDeltaTime = 0.f);
	float GetValue();
};