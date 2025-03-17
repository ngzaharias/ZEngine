#pragma once

namespace math
{
	/// \brief Linearly interpolations from a -> b whilst being frame-rate independent.
	float DamperExact(float x, float g, float halfLife, float deltaTime, float epsilon = 1e-5f);

	/// \brief Extrapolates the future position of a value that has a velocity.
	void Extrapolate(float& x, float& v, float dt, float halflife, float eps = 1e-5f);

	/// \brief Springs a value towards a goal value and velocity without oscillation.
	/// Only accounts for critical damping and not over or under damping.
	void SpringCritical(float& x, float& v, float x_goal, float v_goal, float halflife, float dt);

	/// \brief Springs a value towards a goal value and velocity without oscillation.
	void SpringExact(float& x, float& v, float x_goal, float v_goal, float frequency, float halflife, float dt, float eps = 1e-5f);

	/// \brief Springs a value towards a goal value and velocity with oscillation.
	void SpringRatio(float& x, float& v, float x_goal, float v_goal, float damping_ratio, float halflife, float dt, float eps = 1e-5f);

	/// \brief Spring the value and velocity towards 0.
	void SpringDecay(float& x, float& v, float halflife, float dt);

	/// \brief Spring the value towards its goal value with a goal velocity 0.
	void SpringSimple(float& x, float& v, float x_goal, float halflife, float dt);

	/// \brief Spring the value and velocity towards a goal value and time without oscillation.
	void SpringTimed(float& x, float& v, float& xi, float x_goal, float t_goal, float halflife, float dt);
}