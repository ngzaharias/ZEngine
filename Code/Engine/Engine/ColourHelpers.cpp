#include "EnginePCH.h"
#include "Engine/ColourHelpers.h"

#include "ECS/Entity.h"

#include <type_traits>

namespace
{
	constexpr Vector4f s_Colours[] = {
		// https://lospec.com/palette-list/vanilla-milkshake
		Vector4f(0.97f, 0.50f, 0.51f, 1.00f),
		Vector4f(0.69f, 0.66f, 0.89f, 1.00f),
		Vector4f(0.67f, 0.80f, 0.89f, 1.00f),
		Vector4f(0.70f, 0.89f, 0.85f, 1.00f),
		Vector4f(0.99f, 0.66f, 0.89f, 1.00f),
		Vector4f(0.52f, 0.65f, 0.53f, 1.00f),
		Vector4f(0.69f, 0.92f, 0.57f, 1.00f),
		Vector4f(0.91f, 0.96f, 0.61f, 1.00f),
		Vector4f(1.00f, 0.90f, 0.77f, 1.00f),
		Vector4f(0.87f, 0.63f, 0.54f, 1.00f),
		Vector4f(1.00f, 0.76f, 0.51f, 1.00f),
		Vector4f(1.00f, 0.96f, 0.62f, 1.00f),
		Vector4f(1.00f, 0.96f, 0.89f, 1.00f),
		Vector4f(0.85f, 0.78f, 0.74f, 1.00f),

		// https://lospec.com/palette-list/sweetie-16
		Vector4f(0.69f, 0.24f, 0.32f, 1.00f),
		Vector4f(0.93f, 0.49f, 0.34f, 1.00f),
		Vector4f(1.00f, 0.80f, 0.45f, 1.00f),
		Vector4f(0.65f, 0.94f, 0.43f, 1.00f),
		Vector4f(0.21f, 0.71f, 0.39f, 1.00f),
		Vector4f(0.14f, 0.44f, 0.47f, 1.00f),
		Vector4f(0.16f, 0.21f, 0.43f, 1.00f),
		Vector4f(0.23f, 0.36f, 0.78f, 1.00f),
		Vector4f(0.25f, 0.65f, 0.96f, 1.00f),
		Vector4f(0.45f, 0.93f, 0.96f, 1.00f),
		Vector4f(0.95f, 0.95f, 0.95f, 1.00f),
		Vector4f(0.58f, 0.69f, 0.76f, 1.00f),
		Vector4f(0.33f, 0.42f, 0.52f, 1.00f) };
	constexpr uint32 s_ColoursCount = std::extent<decltype(s_Colours)>::value;
}

Vector3f colour::LerpRGB(const Vector3f& a, const Vector3f& b, const float t)
{
	const Vector3f hsvA = ToHSV(a);
	const Vector3f hsvB = ToHSV(b);
	const Vector3f hsvResult = math::Lerp(hsvA, hsvB, t);
	return ToRGB(hsvResult);
}

Vector3f colour::ToHSV(const Vector3f& rgb)
{
	Vector3f hsv;
	float mrgb, max, delta;

	mrgb = rgb.x < rgb.y ? rgb.x : rgb.y;
	mrgb = mrgb < rgb.z ? mrgb : rgb.z;

	max = rgb.x > rgb.y ? rgb.x : rgb.y;
	max = max > rgb.z ? max : rgb.z;

	hsv.z = max; // v
	delta = max - mrgb;
	if (delta < 0.00001f)
	{
		hsv.y = 0;
		hsv.x = 0; // undefrgbed, maybe nan?
		return hsv;
	}
	// #note: if Max is == 0, this divide would cause a crash
	if (max > 0.f)
	{
		hsv.y = (delta / max); // s
	}
	else
	{
		// if max is 0, then r = g = b = 0
		// s = 0, h is undefrgbed
		hsv.y = 0.f;
		hsv.x = 0.f;
		return hsv;
	}
	if (rgb.x >= max) // > is bogus, just keeps compilor happy
	{
		hsv.x = (rgb.y - rgb.z) / delta; // between yellow & magenta
	}
	else
	{
		if (rgb.y >= max)
		{
			hsv.x = 2.f + (rgb.z - rgb.x) / delta; // between cyan & yellow
		}
		else
		{
			hsv.x = 4.f + (rgb.x - rgb.y) / delta; // between magenta & cyan
		}
	}

	hsv.x *= 60.f; // degrees

	if (hsv.x < 0.f)
		hsv.x += 360.f;
	return hsv;
}

Vector3f colour::ToRGB(const Vector3f& hsv)
{
	float hh, p, q, t, ff;
	long i;
	Vector3f rgb;

	// < is bogus, just shuts up warnhsvgs
	if (hsv.y <= 0.f)
	{
		rgb.x = hsv.z;
		rgb.y = hsv.z;
		rgb.z = hsv.z;
		return rgb;
	}
	hh = hsv.x;
	if (hh >= 360.f) hh = 0.f;
	hh /= 60.f;
	i = (long)hh;
	ff = hh - i;
	p = hsv.z * (1.f - hsv.y);
	q = hsv.z * (1.f - (hsv.y * ff));
	t = hsv.z * (1.f - (hsv.y * (1.f - ff)));

	switch (i)
	{
	case 0:
		rgb.x = hsv.z;
		rgb.y = t;
		rgb.z = p;
		break;
	case 1:
		rgb.x = q;
		rgb.y = hsv.z;
		rgb.z = p;
		break;
	case 2:
		rgb.x = p;
		rgb.y = hsv.z;
		rgb.z = t;
		break;

	case 3:
		rgb.x = p;
		rgb.y = q;
		rgb.z = hsv.z;
		break;
	case 4:
		rgb.x = t;
		rgb.y = p;
		rgb.z = hsv.z;
		break;
	case 5:
	default:
		rgb.x = hsv.z;
		rgb.y = p;
		rgb.z = q;
		break;
	}
	return rgb;
}

const Vector4f& colour::From(const void* value)
{
	auto iValue = reinterpret_cast<std::uintptr_t>(value);
	const uint32 index = iValue % s_ColoursCount;
	return s_Colours[index];
}

const Vector4f& colour::From(const int32 value)
{
	const uint32 index = static_cast<uint32>(value) % s_ColoursCount;
	return s_Colours[index];
}

const Vector4f& colour::From(const uint32 value)
{
	const uint32 index = static_cast<uint32>(value) % s_ColoursCount;
	return s_Colours[index];
}

const Vector4f& colour::From(const ecs::Entity& value)
{
	const uint32 index = static_cast<uint32>(value.m_Value) % s_ColoursCount;
	return s_Colours[index];
}

const Vector4f& colour::Generate()
{
	static uint32 index = 0;
	index = (++index) % s_ColoursCount;
	return s_Colours[index];
}
