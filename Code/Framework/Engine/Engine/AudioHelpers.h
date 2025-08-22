#pragma once

namespace audio
{
	/// \brief Converts a float in the range of [0-1] to a range of [0-100].
	float ToVolume(const float value, const float master = 1.f);
}