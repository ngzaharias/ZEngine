#include "Math/SpringMath.h"

#include "Math/Math.h"

// https://github.com/orangeduck/Spring-It-On
// https://theorangeduck.com/page/spring-roll-call

namespace
{
	constexpr float MAGIC_ONE = 0.69314718056f;
	constexpr float MAGIC_FOUR = MAGIC_ONE * 4.f;

	float fast_negexp(const float x)
	{
		return 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
	}

	float fast_atan(const float x)
	{
		const float z = math::Absolute(x);
		const float w = z > 1.0f ? 1.0f / z : z;
		const float y = PI_QUART * w - w * (w - 1) * (0.2447f + 0.0663f * w);
		return std::copysign(z > 1.0f ? PI_HALF - y : y, x);
	}

	float halflife_to_damping(const float halfLife, const float eps = 1e-5f)
	{
		return MAGIC_FOUR / (halfLife + eps);
	}

	float damping_ratio_to_stiffness(const float ratio, const float damping)
	{
		return math::Sqr(damping / (ratio * 2.0f));
	}

	float frequency_to_stiffness(const float frequency)
	{
		return math::Sqr(PI_TWO * frequency);
	}

	float halflife_to_lag(float halfLife)
	{
		return halfLife / MAGIC_ONE;
	}
}

float math::DamperExact(float x, float g, float halfLife, float dt, float eps)
{
	return math::Lerp(x, g, 1.0f - fast_negexp((NATURAL_LOG_TWO * dt) / (halfLife + eps)));
}

void math::Extrapolate(float& x, float& v, float dt, float halfLife, float eps)
{
	const float y = MAGIC_ONE / (halfLife + eps);
	x = x + (v / (y + eps)) * (1.0f - fast_negexp(y * dt));
	v = v * fast_negexp(y * dt);
}

void math::SpringCritical(float& x, float& v, float x_goal, float v_goal, float halfLife, float dt)
{
	const float g = x_goal;
	const float q = v_goal;
	const float d = halflife_to_damping(halfLife);
	const float c = g + (d * q) / ((d * d) / 4.0f);
	const float y = d / 2.0f;
	const float j0 = x - c;
	const float j1 = v + j0 * y;
	const float eydt = fast_negexp(y * dt);

	x = eydt * (j0 + j1 * dt) + c;
	v = eydt * (v - j1 * y * dt);
}

void math::SpringDecay(float& x, float& v, float halfLife, float dt)
{
	const float y = halflife_to_damping(halfLife) / 2.0f;
	const float j1 = v + x * y;
	const float eydt = fast_negexp(y * dt);

	x = eydt * (x + j1 * dt);
	v = eydt * (v - j1 * y * dt);
}

void math::SpringExact(float& x, float& v, float x_goal, float v_goal, float frequency, float halfLife, float dt, float eps)
{
	const float g = x_goal;
	const float q = v_goal;
	const float s = frequency_to_stiffness(frequency);
	const float d = halflife_to_damping(halfLife);
	const float c = g + (d * q) / (s + eps);
	const float y = d / 2.0f;

	const float sdd4 = s - (d * d) / 4.0f;
	if (math::Absolute(sdd4) < eps) // Critically Damped
	{
		const float j0 = x - c;
		const float j1 = v + j0 * y;
		const float eydt = fast_negexp(y * dt);
		x = j0 * eydt + dt * j1 * eydt + c;
		v = -y * j0 * eydt - y * dt * j1 * eydt + j1 * eydt;
	}
	else if (sdd4 > 0.0) // Under Damped
	{
		float w = math::Sqrt(sdd4);
		float j = math::Sqrt(math::Sqr(v + y * (x - c)) / (w * w + eps) + math::Sqr(x - c));
		float p = fast_atan((v + (x - c) * y) / (-(x - c) * w + eps));

		j = (x - c) > 0.0f ? j : -j;

		const float eydt = fast_negexp(y * dt);
		x = j * eydt * std::cosf(w * dt + p) + c;
		v = -y * j * eydt * std::cosf(w * dt + p) - w * j * eydt * sinf(w * dt + p);
	}
	else if (sdd4 < 0.0) // Over Damped
	{
		const float dd4sqrt = math::Sqrt(d * d - 4 * s);
		const float y0 = (d + dd4sqrt) / 2.0f;
		const float y1 = (d - dd4sqrt) / 2.0f;
		const float j1 = (c * y0 - x * y0 - v) / (y1 - y0);
		const float j0 = x - j1 - c;

		const float ey0dt = fast_negexp(y0 * dt);
		const float ey1dt = fast_negexp(y1 * dt);
		x = j0 * ey0dt + j1 * ey1dt + c;
		v = -y0 * j0 * ey0dt - y1 * j1 * ey1dt;
	}
}

void math::SpringRatio(float& x, float& v, float x_goal, float v_goal, float damping_ratio, float halfLife, float dt, float eps)
{
	const float g = x_goal;
	const float q = v_goal;
	const float d = halflife_to_damping(halfLife);
	const float s = damping_ratio_to_stiffness(damping_ratio, d);
	const float c = g + (d * q) / (s + eps);
	const float y = d / 2.0f;

	const float sdd4 = s - (d * d) / 4.0f;
	if (math::Absolute(sdd4) < eps) // Critically Damped
	{
		const float j0 = x - c;
		const float j1 = v + j0 * y;
		const float eydt = fast_negexp(y * dt);
		x = j0 * eydt + dt * j1 * eydt + c;
		v = -y * j0 * eydt - y * dt * j1 * eydt + j1 * eydt;
	}
	else if (sdd4 > 0.0) // Under Damped
	{
		float w = math::Sqrt(sdd4);
		float j = math::Sqrt(math::Sqr(v + y * (x - c)) / (w * w + eps) + math::Sqr(x - c));
		float p = fast_atan((v + (x - c) * y) / (-(x - c) * w + eps));

		j = (x - c) > 0.0f ? j : -j;

		const float eydt = fast_negexp(y * dt);
		x = j * eydt * std::cosf(w * dt + p) + c;
		v = -y * j * eydt * std::cosf(w * dt + p) - w * j * eydt * sinf(w * dt + p);
	}
	else if (sdd4 < 0.0) // Over Damped
	{
		const float dd4sqrt = math::Sqrt(d * d - 4 * s);
		const float y0 = (d + dd4sqrt) / 2.0f;
		const float y1 = (d - dd4sqrt) / 2.0f;
		const float j1 = (c * y0 - x * y0 - v) / (y1 - y0);
		const float j0 = x - j1 - c;

		const float ey0dt = fast_negexp(y0 * dt);
		const float ey1dt = fast_negexp(y1 * dt);
		x = j0 * ey0dt + j1 * ey1dt + c;
		v = -y0 * j0 * ey0dt - y1 * j1 * ey1dt;
	}
}

void math::SpringSimple(float& x, float& v, float x_goal, float halfLife, float dt)
{
	const float y = halflife_to_damping(halfLife) / 2.0f;
	const float j0 = x - x_goal;
	const float j1 = v + j0 * y;
	const float eydt = fast_negexp(y * dt);

	x = eydt * (j0 + j1 * dt) + x_goal;
	v = eydt * (v - j1 * y * dt);
}

void math::SpringTimed(float& x, float& v, float& xi, float x_goal, float t_goal, float halfLife, float dt)
{
	const float min_time = t_goal > dt ? t_goal : dt;
	const float v_goal = (x_goal - xi) / min_time;
	const float t_goal_future = halflife_to_lag(halfLife);
	const float x_goal_future = t_goal_future < t_goal 
		? xi + v_goal * t_goal_future 
		: x_goal;

	SpringSimple(x, v, x_goal_future, halfLife, dt);
	xi += v_goal * dt;
}
