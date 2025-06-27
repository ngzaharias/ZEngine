#pragma once

#include "Math/Vector.h"

class Colour
{
public:
	constexpr Colour() noexcept 
		: r(), g(), b(), a() { }
	constexpr Colour(const float rgb, const float alpha = 1.f) noexcept
		: r(rgb), g(rgb), b(rgb), a(alpha) { }
	constexpr Colour(const float red, const float green, const float blue, const float alpha = 1.f) noexcept 
		: r(red), g(green), b(blue), a(alpha) { }
	constexpr Colour(const Vector3f& rgb, const float alpha = 1.f) noexcept
		: r(rgb.x), g(rgb.y), b(rgb.z), a(alpha) { }
	constexpr Colour(const Vector4f& rgba) noexcept
		: r(rgba.x), g(rgba.y), b(rgba.z), a(rgba.w) { }

	operator Vector4f() const { return Vector4f(r, g, b, a); }

	static const Colour Black;
	static const Colour Blue;
	static const Colour Cyan;
	static const Colour Green;
	static const Colour Magenta;
	static const Colour Red;
	static const Colour White;
	static const Colour Yellow;

public:
	float r, g, b, a;
};

inline constexpr Colour Colour::Black(0.f, 0.f, 0.f, 1.f);
inline constexpr Colour Colour::Blue(0.f, 0.f, 1.f, 1.f);
inline constexpr Colour Colour::Cyan(0.f, 1.f, 1.f, 1.f);
inline constexpr Colour Colour::Green(0.f, 1.f, 0.f, 1.f);
inline constexpr Colour Colour::Magenta(1.f, 0.f, 1.f, 1.f);
inline constexpr Colour Colour::Red(1.f, 0.f, 0.f, 1.f);
inline constexpr Colour Colour::White(1.f, 1.f, 1.f, 1.f);
inline constexpr Colour Colour::Yellow(1.f, 1.f, 0.f, 1.f);