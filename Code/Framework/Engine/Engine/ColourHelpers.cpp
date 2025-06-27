#include "EnginePCH.h"
#include "Engine/ColourHelpers.h"

#include "Core/Colour.h"
#include "ECS/Entity.h"

#include <type_traits>

namespace
{
	constexpr Colour s_Colours[] = {
		// https://lospec.com/palette-list/vanilla-milkshake
		Colour(0.97f, 0.50f, 0.51f, 1.00f),
		Colour(0.69f, 0.66f, 0.89f, 1.00f),
		Colour(0.67f, 0.80f, 0.89f, 1.00f),
		Colour(0.70f, 0.89f, 0.85f, 1.00f),
		Colour(0.99f, 0.66f, 0.89f, 1.00f),
		Colour(0.52f, 0.65f, 0.53f, 1.00f),
		Colour(0.69f, 0.92f, 0.57f, 1.00f),
		Colour(0.91f, 0.96f, 0.61f, 1.00f),
		Colour(1.00f, 0.90f, 0.77f, 1.00f),
		Colour(0.87f, 0.63f, 0.54f, 1.00f),
		Colour(1.00f, 0.76f, 0.51f, 1.00f),
		Colour(1.00f, 0.96f, 0.62f, 1.00f),
		Colour(1.00f, 0.96f, 0.89f, 1.00f),
		Colour(0.85f, 0.78f, 0.74f, 1.00f),

		// https://lospec.com/palette-list/sweetie-16
		Colour(0.69f, 0.24f, 0.32f, 1.00f),
		Colour(0.93f, 0.49f, 0.34f, 1.00f),
		Colour(1.00f, 0.80f, 0.45f, 1.00f),
		Colour(0.65f, 0.94f, 0.43f, 1.00f),
		Colour(0.21f, 0.71f, 0.39f, 1.00f),
		Colour(0.14f, 0.44f, 0.47f, 1.00f),
		Colour(0.16f, 0.21f, 0.43f, 1.00f),
		Colour(0.23f, 0.36f, 0.78f, 1.00f),
		Colour(0.25f, 0.65f, 0.96f, 1.00f),
		Colour(0.45f, 0.93f, 0.96f, 1.00f),
		Colour(0.95f, 0.95f, 0.95f, 1.00f),
		Colour(0.58f, 0.69f, 0.76f, 1.00f),
		Colour(0.33f, 0.42f, 0.52f, 1.00f) };
	constexpr uint32 s_ColoursCount = std::extent<decltype(s_Colours)>::value;
}

Colour colour::LerpRGB(const Colour& a, const Colour& b, const float t)
{
	const Colour hsvA = ToHSV(a);
	const Colour hsvB = ToHSV(b);
	const Colour hsvC = math::Lerp<Vector4f>(hsvA, hsvB, t);
	return ToRGB(hsvC);
}

Colour colour::ToHSV(const Colour& rgb)
{
	Colour hsv;
	float mrgb, max, delta;

	mrgb = rgb.r < rgb.g ? rgb.r : rgb.g;
	mrgb = mrgb < rgb.b ? mrgb : rgb.b;

	max = rgb.r > rgb.g ? rgb.r : rgb.g;
	max = max > rgb.b ? max : rgb.b;

	hsv.b = max;
	delta = max - mrgb;
	if (delta < 0.00001f)
	{
		hsv.g = 0;
		hsv.r = 0; 
		return hsv;
	}
	// #note: if max is == 0, this divide would cause a crash
	if (max > 0.f)
	{
		hsv.g = (delta / max);
	}
	else
	{
		// if max is 0, then r = g = b = 0
		hsv.g = 0.f;
		hsv.r = 0.f;
		return hsv;
	}
	if (rgb.r >= max)
	{
		hsv.r = (rgb.g - rgb.b) / delta; 
	}
	else
	{
		if (rgb.g >= max)
		{
			hsv.r = 2.f + (rgb.b - rgb.r) / delta;
		}
		else
		{
			hsv.r = 4.f + (rgb.r - rgb.g) / delta;
		}
	}

	hsv.r *= 60.f; // degrees

	if (hsv.r < 0.f)
		hsv.r += 360.f;
	hsv.a = 1.f;
	return hsv;
}

Colour colour::ToRGB(const Colour& hsv)
{
	float hh, p, q, t, ff;
	long i;
	Colour rgb;

	if (hsv.g <= 0.f)
	{
		rgb.r = hsv.b;
		rgb.g = hsv.b;
		rgb.b = hsv.b;
		return rgb;
	}
	hh = hsv.r;
	if (hh >= 360.f) hh = 0.f;
	hh /= 60.f;
	i = (long)hh;
	ff = hh - i;
	p = hsv.b * (1.f - hsv.g);
	q = hsv.b * (1.f - (hsv.g * ff));
	t = hsv.b * (1.f - (hsv.g * (1.f - ff)));

	switch (i)
	{
	case 0:
		rgb.r = hsv.b;
		rgb.g = t;
		rgb.b = p;
		break;
	case 1:
		rgb.r = q;
		rgb.g = hsv.b;
		rgb.b = p;
		break;
	case 2:
		rgb.r = p;
		rgb.g = hsv.b;
		rgb.b = t;
		break;

	case 3:
		rgb.r = p;
		rgb.g = q;
		rgb.b = hsv.b;
		break;
	case 4:
		rgb.r = t;
		rgb.g = p;
		rgb.b = hsv.b;
		break;
	case 5:
	default:
		rgb.r = hsv.b;
		rgb.g = p;
		rgb.b = q;
		break;
	}
	rgb.a = 1.f;
	return rgb;
}

const Colour& colour::From(const void* value)
{
	auto iValue = reinterpret_cast<std::uintptr_t>(value);
	const uint32 index = iValue % s_ColoursCount;
	return s_Colours[index];
}

const Colour& colour::From(const int32 value)
{
	const uint32 index = static_cast<uint32>(value) % s_ColoursCount;
	return s_Colours[index];
}

const Colour& colour::From(const uint32 value)
{
	const uint32 index = static_cast<uint32>(value) % s_ColoursCount;
	return s_Colours[index];
}

const Colour& colour::From(const ecs::Entity& value)
{
	const uint32 index = static_cast<uint32>(value.m_Value) % s_ColoursCount;
	return s_Colours[index];
}

const Colour& colour::Generate()
{
	static uint32 index = 0;
	index = (++index) % s_ColoursCount;
	return s_Colours[index];
}
