#pragma once
// used: [stl] numeric_limits
#include <limits>
// used: [crt] isfinite, fmodf, sqrtf
#include <cmath>

// forward declarations
struct QAngle;
struct Matrix3x4;

// @source: master/public/mathlib/vector.h

struct Vector2D
{
	constexpr Vector2D(const float x = 0.0f, const float y = 0.0f) :
		x(x), y(y) { }

	[[nodiscard]] bool IsZero() const
	{
		// @note: to make this implementation right, we should use fpclassify here, but game aren't doing same, probably it's better to keep this same, just ensure that it will be compiled same
		return (this->x == 0.0f && this->y == 0.0f);
	}

	[[nodiscard]] float Length() const
	{
		return std::sqrtf(this->LengthSqr());
	}

	[[nodiscard]] constexpr float DotProduct(const Vector2D& vecDot) const
	{
		return (this->x * vecDot.x + this->y * vecDot.y);
	}

	[[nodiscard]] constexpr float LengthSqr() const
	{
		return DotProduct(*this);
	}

	Vector2D operator+(const Vector2D& vecAdd) const
	{
		return { this->x + vecAdd.x, this->y + vecAdd.y };
	}

	Vector2D operator-(const Vector2D& vecSubtract) const
	{
		return { this->x - vecSubtract.x, this->y - vecSubtract.y };
	}

	Vector2D operator*(const Vector2D& vecMultiply) const
	{
		return { this->x * vecMultiply.x, this->y * vecMultiply.y };
	}

	Vector2D operator/(const Vector2D& vecDivide) const
	{
		return { this->x / vecDivide.x, this->y / vecDivide.y };
	}

	Vector2D operator+(const float flAdd) const
	{
		return { this->x + flAdd, this->y + flAdd };
	}

	Vector2D operator-(const float flSubtract) const
	{
		return { this->x - flSubtract, this->y - flSubtract };
	}

	Vector2D operator*(const float flMultiply) const
	{
		return { this->x * flMultiply, this->y * flMultiply };
	}

	Vector2D operator/(const float flDivide) const
	{
		return { this->x / flDivide, this->y / flDivide };
	}

	constexpr Vector2D& operator+=(const Vector2D& vecBase)
	{
		this->x += vecBase.x;
		this->y += vecBase.y;
		return *this;
	}

	constexpr Vector2D& operator-=(const Vector2D& vecBase)
	{
		this->x -= vecBase.x;
		this->y -= vecBase.y;
		return *this;
	}

	constexpr Vector2D& operator*=(const Vector2D& vecBase)
	{
		this->x *= vecBase.x;
		this->y *= vecBase.y;
		return *this;
	}

	constexpr Vector2D& operator/=(const Vector2D& vecBase)
	{
		this->x /= vecBase.x;
		this->y /= vecBase.y;
		return *this;
	}

	constexpr Vector2D& operator+=(const float flAdd)
	{
		this->x += flAdd;
		this->y += flAdd;
		return *this;
	}

	constexpr Vector2D& operator-=(const float flSubtract)
	{
		this->x -= flSubtract;
		this->y -= flSubtract;
		return *this;
	}

	constexpr Vector2D& operator*=(const float flMultiply)
	{
		this->x *= flMultiply;
		this->y *= flMultiply;
		return *this;
	}

	constexpr Vector2D& operator/=(const float flDivide)
	{
		this->x /= flDivide;
		this->y /= flDivide;
		return *this;
	}

	float x = 0.0f, y = 0.0f;
};

struct Vector
{
	constexpr Vector(const float x = 0.0f, const float y = 0.0f, const float z = 0.0f) :
		x(x), y(y), z(z) { }

	constexpr Vector(const float* arrVector) :
		x(arrVector[0]), y(arrVector[1]), z(arrVector[2]) { }

	constexpr Vector(const Vector2D& vecBase2D) :
		x(vecBase2D.x), y(vecBase2D.y) { }

#pragma region vector_array_operators

	[[nodiscard]] float& operator[](const int nIndex)
	{
		return reinterpret_cast<float*>(this)[nIndex];
	}

	[[nodiscard]] const float& operator[](const int nIndex) const
	{
		return reinterpret_cast<const float*>(this)[nIndex];
	}

	Vector Lerp(const Vector& other, float t) const {
		return Vector(x + t * (other.x - x),
			y + t * (other.y - y),
			z + t * (other.z - z));
	}

#pragma endregion

#pragma region vector_relational_operators

	bool operator==(const Vector& vecBase) const
	{
		return this->IsEqual(vecBase);
	}

	bool operator!=(const Vector& vecBase) const
	{
		return !this->IsEqual(vecBase);
	}

#pragma endregion

#pragma region vector_assignment_operators

	constexpr Vector& operator=(const Vector& vecBase)
	{
		this->x = vecBase.x;
		this->y = vecBase.y;
		this->z = vecBase.z;
		return *this;
	}

	constexpr Vector& operator=(const Vector2D& vecBase2D)
	{
		this->x = vecBase2D.x;
		this->y = vecBase2D.y;
		this->z = 0.0f;
		return *this;
	}

#pragma endregion

#pragma region vector_arithmetic_assignment_operators

	constexpr Vector& operator+=(const Vector& vecBase)
	{
		this->x += vecBase.x;
		this->y += vecBase.y;
		this->z += vecBase.z;
		return *this;
	}

	constexpr Vector& operator-=(const Vector& vecBase)
	{
		this->x -= vecBase.x;
		this->y -= vecBase.y;
		this->z -= vecBase.z;
		return *this;
	}

	constexpr Vector& operator*=(const Vector& vecBase)
	{
		this->x *= vecBase.x;
		this->y *= vecBase.y;
		this->z *= vecBase.z;
		return *this;
	}

	constexpr Vector& operator/=(const Vector& vecBase)
	{
		this->x /= vecBase.x;
		this->y /= vecBase.y;
		this->z /= vecBase.z;
		return *this;
	}

	constexpr Vector& operator+=(const float flAdd)
	{
		this->x += flAdd;
		this->y += flAdd;
		this->z += flAdd;
		return *this;
	}

	constexpr Vector& operator-=(const float flSubtract)
	{
		this->x -= flSubtract;
		this->y -= flSubtract;
		this->z -= flSubtract;
		return *this;
	}

	constexpr Vector& operator*=(const float flMultiply)
	{
		this->x *= flMultiply;
		this->y *= flMultiply;
		this->z *= flMultiply;
		return *this;
	}

	constexpr Vector& operator/=(const float flDivide)
	{
		this->x /= flDivide;
		this->y /= flDivide;
		this->z /= flDivide;
		return *this;
	}

#pragma endregion

#pragma region vector_arithmetic_unary_operators

	constexpr Vector& operator-()
	{
		this->x = -this->x;
		this->y = -this->y;
		this->z = -this->z;
		return *this;
	}

	constexpr Vector operator-() const
	{
		return { -this->x, -this->y, -this->z };
	}

#pragma endregion

#pragma region vector_arithmetic_ternary_operators

	Vector operator+(const Vector& vecAdd) const
	{
		return { this->x + vecAdd.x, this->y + vecAdd.y, this->z + vecAdd.z };
	}

	Vector operator-(const Vector& vecSubtract) const
	{
		return { this->x - vecSubtract.x, this->y - vecSubtract.y, this->z - vecSubtract.z };
	}

	Vector operator*(const Vector& vecMultiply) const
	{
		return { this->x * vecMultiply.x, this->y * vecMultiply.y, this->z * vecMultiply.z };
	}

	Vector operator/(const Vector& vecDivide) const
	{
		return { this->x / vecDivide.x, this->y / vecDivide.y, this->z / vecDivide.z };
	}

	Vector operator+(const float flAdd) const
	{
		return { this->x + flAdd, this->y + flAdd, this->z + flAdd };
	}

	Vector operator-(const float flSubtract) const
	{
		return { this->x - flSubtract, this->y - flSubtract, this->z - flSubtract };
	}

	Vector operator*(const float flMultiply) const
	{
		return { this->x * flMultiply, this->y * flMultiply, this->z * flMultiply };
	}

	Vector operator/(const float flDivide) const
	{
		return { this->x / flDivide, this->y / flDivide, this->z / flDivide };
	}

#pragma endregion

	/// @returns: true if each component of the vector is finite, false otherwise
	[[nodiscard]] bool IsValid() const
	{
		return std::isfinite(this->x) && std::isfinite(this->y) && std::isfinite(this->z);
	}

	constexpr void Invalidate()
	{
		this->x = this->y = this->z = std::numeric_limits<float>::infinity();
	}

	/// @returns: true if each component of the vector equals to another, false otherwise
	[[nodiscard]] bool IsEqual(const Vector& vecEqual, const float flErrorMargin = std::numeric_limits<float>::epsilon()) const
	{
		return (std::fabsf(this->x - vecEqual.x) < flErrorMargin && std::fabsf(this->y - vecEqual.y) < flErrorMargin && std::fabsf(this->z - vecEqual.z) < flErrorMargin);
	}

	/// @returns: true if each component of the vector equals to zero, false otherwise
	[[nodiscard]] bool IsZero() const
	{
		// @note: to make this implementation right, we should use fpclassify here, but game aren't doing same, probably it's better to keep this same, just ensure that it will be compiled same
		return (this->x == 0.0f && this->y == 0.0f && this->z == 0.0f);
	}

	[[nodiscard]] float Length() const
	{
		return std::sqrtf(this->LengthSqr());
	}

	[[nodiscard]] constexpr float LengthSqr() const
	{
		return DotProduct(*this);
	}

	[[nodiscard]] float Length2D() const
	{
		return std::sqrtf(this->Length2DSqr());
	}

	[[nodiscard]] constexpr float Length2DSqr() const
	{
		return (this->x * this->x + this->y * this->y);
	}

	[[nodiscard]] float DistTo(const Vector& vecEnd) const
	{
		return (*this - vecEnd).Length();
	}

	[[nodiscard]] float DistTo2D(const Vector& vecEnd) const
	{
		return (*this - vecEnd).Length2D();
	}

	[[nodiscard]] constexpr float DistToSqr(const Vector& vecEnd) const
	{
		return (*this - vecEnd).LengthSqr();
	}

	/// normalize magnitude of each component of the vector
	/// @returns: length of the vector
	float NormalizeInPlace()
	{
		const float flLength = this->Length();
		const float flRadius = 1.0f / (flLength + std::numeric_limits<float>::epsilon());

		this->x *= flRadius;
		this->y *= flRadius;
		this->z *= flRadius;

		return flLength;
	}

	/// normalize magnitude of each component of the vector
	/// @returns: copy of the vector with normalized components
	[[nodiscard]] Vector Normalized() const
	{
		Vector vecOut = *this;
		vecOut.NormalizeInPlace();
		return vecOut;
	}

	void normalize() {
		float mag = sqrt(x * x + y * y + z * z);
		if (mag != 0) {
			x /= mag;
			y /= mag;
			z /= mag;
		}
	}

	[[nodiscard]] constexpr float DotProduct(const Vector& vecDot) const
	{
		return (this->x * vecDot.x + this->y * vecDot.y + this->z * vecDot.z);
	}

	[[nodiscard]] constexpr Vector CrossProduct(const Vector& vecCross) const
	{
		return { this->y * vecCross.z - this->z * vecCross.y, this->z * vecCross.x - this->x * vecCross.z, this->x * vecCross.y - this->y * vecCross.x };
	}

	[[nodiscard]] Vector2D ToVector2D() const
	{
		return { this->x, this->y };
	}

	/// convert forward direction vector to other direction vectors
	/// @param[out] pvecRight [optional] output for converted right vector
	/// @param[out] pvecUp [optional] output for converted up vector
	void ToDirections(Vector* pvecRight, Vector* pvecUp) const
	{
		if (std::fabsf(this->x) < 1e-6f && std::fabsf(this->y) < 1e-6f)
		{
			// pitch 90 degrees up/down from identity
			if (pvecRight != nullptr)
			{
				pvecRight->x = 0.0f;
				pvecRight->y = -1.0f;
				pvecRight->z = 0.0f;
			}

			if (pvecUp != nullptr)
			{
				pvecUp->x = -this->z;
				pvecUp->y = 0.0f;
				pvecUp->z = 0.0f;
			}
		}
		else
		{
			if (pvecRight != nullptr)
			{
				pvecRight->x = this->y;
				pvecRight->y = -this->x;
				pvecRight->z = 0.0f;
				pvecRight->NormalizeInPlace();
			}

			if (pvecUp != nullptr)
			{
				pvecUp->x = (-this->x) * this->z;
				pvecUp->y = -(this->y * this->z);
				pvecUp->z = this->y * this->y - (-this->x) * this->x;
				pvecUp->NormalizeInPlace();
			}
		}
	}

	/// @returns: 2D angles converted from direction vector
	[[nodiscard]] QAngle ToAngles() const;

	QAngle ToAngles(const Vector& vUp) const;

	float x = 0.0f, y = 0.0f, z = 0.0f;
};

struct Vector4D
{
	constexpr Vector4D(const float x = 0.0f, const float y = 0.0f, const float z = 0.0f, const float w = 0.0f) :
		x(x), y(y), z(z), w(w) { }

	float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;
};

struct alignas(16) VectorAligned : Vector
{
	VectorAligned() = default;

	explicit VectorAligned(const Vector& vecBase)
	{
		this->x = vecBase.x;
		this->y = vecBase.y;
		this->z = vecBase.z;
		this->w = 0.0f;
	}

	constexpr VectorAligned& operator=(const Vector& vecBase)
	{
		this->x = vecBase.x;
		this->y = vecBase.y;
		this->z = vecBase.z;
		this->w = 0.0f;
		return *this;
	}

	float w = 0.0f;
};

static_assert(alignof(VectorAligned) == 16);