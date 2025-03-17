#pragma once

namespace math
{
	/// \brief Linearly interpolations from a -> b using a halfLife that is updated using delta time.
	/// Increase the epsilon when working with low precision to avoid division by zero.
	float DamperExact(float a, float b, float halfLife, float deltaTime, float epsilon = 1e-5f);
}