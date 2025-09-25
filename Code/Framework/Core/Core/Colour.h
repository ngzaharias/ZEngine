#pragma once

#include "Math/Vector.h"

class Colour
{
public:
	constexpr Colour() noexcept 
		: r(), g(), b(), a() { }
	constexpr Colour(const uint32 rgb) noexcept
		: r(((rgb >> 16) & 0xFF) / 255.f)
		, g(((rgb >> 8) & 0xFF) / 255.f)
		, b(((rgb >> 0) & 0xFF) / 255.f)
		, a(1.f)
	{ }
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
	static const Colour Gray;
	static const Colour Green;
	static const Colour Magenta;
	static const Colour Red;
	static const Colour White;
	static const Colour Yellow;

public:
	float r, g, b, a;
};

inline constexpr Colour Colour::Black((uint32)0x000000);
inline constexpr Colour Colour::Blue((uint32)0x0000FF);
inline constexpr Colour Colour::Cyan((uint32)0x00FFFF);
inline constexpr Colour Colour::Gray((uint32)0x808080);
inline constexpr Colour Colour::Green((uint32)0x00FF00);
inline constexpr Colour Colour::Magenta((uint32)0xFF00FF);
inline constexpr Colour Colour::Red((uint32)0xFF0000);
inline constexpr Colour Colour::White((uint32)0xFFFFFF);
inline constexpr Colour Colour::Yellow((uint32)0xFFFF00);