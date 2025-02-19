// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "vector.h"

#include "qangle.h"

// used: m_rad2deg
#include <DirectXMath.h>

[[nodiscard]] QAngle Vector::ToAngles() const
{
	float flPitch, flYaw;
	if (this->x == 0.0f && this->y == 0.0f)
	{
		flPitch = (this->z > 0.0f) ? 270.f : 90.f;
		flYaw = 0.0f;
	}
	else
	{
		flPitch = DirectX::XMConvertToDegrees(std::atan2f(-this->z, this->Length2D()));

		if (flPitch < 0.f)
			flPitch += 360.f;

		flYaw = DirectX::XMConvertToDegrees(std::atan2f(this->y, this->x));

		if (flYaw < 0.f)
			flYaw += 360.f;
	}

	return { flPitch, flYaw, 0.0f };
}

QAngle Vector::ToAngles(const Vector& vUp) const {
	Vector vLeft = CrossProduct(vUp);
	vLeft.NormalizeInPlace();

	float flForwardDistance = this->Length2D();
	if (flForwardDistance > 0.001f) {
		float upZ = (vLeft.y * this->x) - (vLeft.x * this->y);
		return {
			atan2f(-this->z, flForwardDistance) * 180 / DirectX::XM_PI,
			atan2f(this->y, this->x) * 180 / DirectX::XM_PI,
			atan2f(vLeft.z, upZ) * 180 / DirectX::XM_PI

		};
	}
	else {
		return {
			atan2f(-this->z, flForwardDistance) * 180 / DirectX::XM_PI,
			atan2f(-vLeft.x, vLeft.y) * 180 / DirectX::XM_PI,
			0.0f
		};
	}
}