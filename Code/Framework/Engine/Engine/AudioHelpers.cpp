#include "EnginePCH.h"
#include "Engine/AudioHelpers.h"

#include "Math/Math.h"

float audio::ToVolume(const float value, const float master)
{
	return std::powf(value * master, EULERS_CONSTANT) * 100.f;
}
