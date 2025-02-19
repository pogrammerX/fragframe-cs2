#pragma once
#include <cstdint>
#include <bit>

enum
{
	COLOR_R = 0,
	COLOR_G = 1,
	COLOR_B = 2,
	COLOR_A = 3
};

struct Color
{
	Color() = default;

	// 8-bit color constructor (in: [0 .. 255])
	constexpr Color(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a = 255) :
		r(r), g(g), b(b), a(a) { }

	constexpr Color(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, float a) :
		r(r), g(g), b(b), a(a * 255.f) { }

	constexpr Color(const std::uint8_t v, std::uint8_t s = 255) :
		r(v), g(v), b(v), a(s) { }

	// 8-bit color constructor (in: [0 .. 255])
	constexpr Color(const int r, const int g, const int b, const int a = 255) :
		r(static_cast<std::uint8_t>(r)), g(static_cast<std::uint8_t>(g)), b(static_cast<std::uint8_t>(b)), a(static_cast<std::uint8_t>(a)) { }

	// 8-bit array color constructor (in: [0.0 .. 1.0])
	explicit constexpr Color(const std::uint8_t arrColor[4]) :
		r(arrColor[COLOR_R]), g(arrColor[COLOR_G]), b(arrColor[COLOR_B]), a(arrColor[COLOR_A]) { }

	// 32-bit color constructor (in: [0.0 .. 1.0])
	constexpr Color(const float r, const float g, const float b, const float a = 1.0f) :
		r(static_cast<std::uint8_t>(r * 255.f)), g(static_cast<std::uint8_t>(g * 255.f)), b(static_cast<std::uint8_t>(b * 255.f)), a(static_cast<std::uint8_t>(a * 255.f)) { }

	std::uint8_t& operator[](const std::uint8_t nIndex)
	{
		return reinterpret_cast<std::uint8_t*>(this)[nIndex];
	}

	const std::uint8_t& operator[](const std::uint8_t nIndex) const
	{
		return reinterpret_cast<const std::uint8_t*>(this)[nIndex];
	}

	bool operator==(const Color& colSecond) const
	{
		return (std::bit_cast<std::uint32_t>(*this) == std::bit_cast<std::uint32_t>(colSecond));
	}

	bool operator!=(const Color& colSecond) const
	{
		return (std::bit_cast<std::uint32_t>(*this) != std::bit_cast<std::uint32_t>(colSecond));
	}

	/// @returns: copy of color with certain R/G/B/A component changed to given value
	template <std::size_t N>
	[[nodiscard]] Color Set(const std::uint8_t nValue) const
	{
		static_assert(N >= COLOR_R && N <= COLOR_A, "color component index is out of range");

		Color colCopy = *this;
		colCopy[N] = nValue;
		return colCopy;
	}

	/// @returns: copy of color with certain R/G/B/A component multiplied by given value
	template <std::size_t N>
	[[nodiscard]] Color Multiplier(const float flValue) const
	{
		static_assert(N >= COLOR_R && N <= COLOR_A, "color component index is out of range");

		Color colCopy = *this;
		colCopy[N] = static_cast<std::uint8_t>(static_cast<float>(colCopy[N]) * flValue);
		return colCopy;
	}

	/// @returns: copy of color with certain R/G/B/A component divided by given value
	template <std::size_t N>
	[[nodiscard]] Color Divider(const int iValue) const
	{
		static_assert(N >= COLOR_R && N <= COLOR_A, "color component index is out of range");

		Color colCopy = *this;
		colCopy[N] /= iValue;
		return colCopy;
	}

	/// @returns: certain R/G/B/A float value (in: [0 .. 255], out: [0.0 .. 1.0])
	template <std::size_t N>
	[[nodiscard]] float Base() const
	{
		static_assert(N >= COLOR_R && N <= COLOR_A, "color component index is out of range");
		return reinterpret_cast<const std::uint8_t*>(this)[N] / 255.f;
	}

	/// @param[out] arrBase output array of R/G/B color components converted to float (in: [0 .. 255], out: [0.0 .. 1.0])
	constexpr void Base(float(&arrBase)[3]) const
	{
		arrBase[COLOR_R] = static_cast<float>(r) / 255.f;
		arrBase[COLOR_G] = static_cast<float>(g) / 255.f;
		arrBase[COLOR_B] = static_cast<float>(b) / 255.f;
	}

	/// @returns: color created from float[3] array (in: [0.0 .. 1.0], out: [0 .. 255])
	static Color FromBase3(const float arrBase[3])
	{
		return { arrBase[0], arrBase[1], arrBase[2] };
	}

	/// @param[out] arrBase output array of R/G/B/A color components converted to float (in: [0 .. 255], out: [0.0 .. 1.0])
	constexpr void BaseAlpha(float(&arrBase)[4]) const
	{
		arrBase[COLOR_R] = static_cast<float>(r) / 255.f;
		arrBase[COLOR_G] = static_cast<float>(g) / 255.f;
		arrBase[COLOR_B] = static_cast<float>(b) / 255.f;
		arrBase[COLOR_A] = static_cast<float>(a) / 255.f;
	}

	/// @returns : color created from float[3] array (in: [0.0 .. 1.0], out: [0 .. 255])
	static Color FromBase4(const float arrBase[4])
	{
		return { arrBase[COLOR_R], arrBase[COLOR_G], arrBase[COLOR_B], arrBase[COLOR_A] };
	}

	std::uint8_t r = 0U, g = 0U, b = 0U, a = 0U;
};