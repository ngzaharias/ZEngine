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

	const Colour& Generate();
	const Colour& Generate(const void* value);
	const Colour& Generate(const int32 value);
	const Colour& Generate(const uint32 value);
	const Colour& Generate(const ecs::Entity& value);
}
