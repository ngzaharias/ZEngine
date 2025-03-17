#include "Math/SpringMath.h"

#include "Math/Math.h"

// https://theorangeduck.com/page/spring-roll-call

namespace
{
	// fast negative exponent
	float NegExp(float x)
	{
		return 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
	}
}

float math::DamperExact(float a, float b, float halfLife, float deltaTime, float epsilon = 1e-5f)
{
	return math::Lerp(a, b, 1.0f - NegExp((NATURAL_LOG_TWO * deltaTime) / (halfLife + epsilon)));
}
