#pragma once

class Colour;

namespace ecs
{
	struct Entity;
}

namespace colour
{
	Colour LerpRGB(const Colour& a, const Colour& b, const float t);

	Colour ToHSV(const Colour& rgb);
	Colour ToRGB(const Colour& hsv);

	const Colour& From(const void* value);
	const Colour& From(const int32 value);
	const Colour& From(const uint32 value);
	const Colour& From(const ecs::Entity& value);

	const Colour& Generate();
}
