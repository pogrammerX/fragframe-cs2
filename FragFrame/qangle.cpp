#include "qangle.h"

// used: [d3d] xmscalarsincos
#include <directxmath.h>

void QAngle::ToDirections(Vector* pvecForward, Vector* pvecRight, Vector* pvecUp) const
{
	float flPitchSin, flPitchCos, flYawSin, flYawCos, flRollSin, flRollCos;
	DirectX::XMScalarSinCos(&flPitchSin, &flPitchCos, DirectX::XMConvertToRadians(this->x));
	DirectX::XMScalarSinCos(&flYawSin, &flYawCos, DirectX::XMConvertToRadians(this->y));
	DirectX::XMScalarSinCos(&flRollSin, &flRollCos, DirectX::XMConvertToRadians(this->z));

	if (pvecForward != nullptr)
	{
		pvecForward->x = flPitchCos * flYawCos;
		pvecForward->y = flPitchCos * flYawSin;
		pvecForward->z = -flPitchSin;
	}

	if (pvecRight != nullptr)
	{
		pvecRight->x = (-flRollSin * flPitchSin * flYawCos) + (-flRollCos * -flYawSin);
		pvecRight->y = (-flRollSin * flPitchSin * flYawSin) + (-flRollCos * flYawCos);
		pvecRight->z = (-flRollSin * flPitchCos);
	}

	if (pvecUp != nullptr)
	{
		pvecUp->x = (flRollCos * flPitchSin * flYawCos) + (-flRollSin * -flYawSin);
		pvecUp->y = (flRollCos * flPitchSin * flYawSin) + (-flRollSin * flYawCos);
		pvecUp->z = (flRollCos * flPitchCos);
	}
}