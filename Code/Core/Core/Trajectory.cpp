#include "Core/Trajectory.h"

#include "Core/VariantHelpers.h"
#include "Math/Math.h"
#include "Math/VectorMath.h"

// http://www.planetclegg.com/projects/WarpingTextToSplines.html
// https://gamedev.stackexchange.com/questions/5373/moving-ships-between-two-planets-along-a-bezier-missing-some-equations-for-acce/5427#5427

namespace
{
	Vector3f Bezier(const Vector3f& p0, const Vector3f& c, const Vector3f& p2, float t)
	{
		const Vector3f ip0 = math::Lerp(p0, c, t);
		const Vector3f ip1 = math::Lerp(c, p2, t);
		return math::Lerp(ip0, ip1, t);
	}

	Vector3f Bezier(const Vector3f& p0, const Vector3f& c0, const Vector3f& c1, const Vector3f& p3, float t)
	{
		const Vector3f ip0 = math::Lerp(p0, c0, t);
		const Vector3f ip1 = math::Lerp(c0, c1, t);
		const Vector3f ip2 = math::Lerp(c1, p3, t);
		const Vector3f ip3 = math::Lerp(ip0, ip1, t);
		const Vector3f ip4 = math::Lerp(ip1, ip2, t);
		return math::Lerp(ip3, ip4, t);
	}

	// remap a distance to its associated interpolation value
	float ToInterpolatedValue(const float distance, const Slice<const float> distances)
	{
		const float distanceMax = distances.GetLast();
		if (distanceMax <= 0.f)
			return 1.f;
		if (distance <= 0.f)
			return 0.f;
		if (distance >= distanceMax)
			return 1.f;

		// #todo: binary search
		const int32 count = distances.GetCount();
		for (int32 i = 1; i < count; ++i)
		{
			const float prev = distances[i - 1];
			const float curr = distances[i];
			if (prev <= distance && distance <= curr)
			{
				const float a = (i - 1.f) / (count - 1.f);
				const float b = i / (count - 1.f);
				return math::Remap(distance, prev, curr, a, b);
			}
		}
		return 1.f;
	}

	Vector3f ToPosition(const path::Points& data, const float distance)
	{
		if (data.m_Positions.IsEmpty())
			return Vector3f::Zero;

		const float distanceMax = data.m_Distances.GetLast();
		if (distanceMax <= 0.f)
			return data.m_Positions.GetLast();
		if (distance <= 0.f)
			return data.m_Positions.GetFirst();
		if (distance >= distanceMax)
			return data.m_Positions.GetLast();

		float prevDistance = -1.f;
		float nextDistance = -1.f;
		Vector3f prevPosition = data.m_Positions.GetFirst();
		Vector3f nextPosition = prevPosition;

		// #todo: binary search
		const int32 count = data.m_Positions.GetCount();
		for (int32 i = 0; i < count; ++i)
		{
			prevDistance = nextDistance;
			prevPosition = nextPosition;
			nextDistance = data.m_Distances[i];
			nextPosition = data.m_Positions[i];
			if (prevDistance <= distance && distance <= nextDistance)
			{
				const float t = math::Remap(distance, prevDistance, nextDistance, 0.f, 1.f);
				return math::Lerp(prevPosition, nextPosition, t);
			}
		}

		return Vector3f::Zero;
	};
}

path::Trajectory::Trajectory()
	: m_Mappings(0.f)
	, m_Settings()
{
}

float path::Trajectory::GetLength() const
{
	return m_Mappings.GetLast();
}

path::Trajectory::Trajectory(const Settings& settings)
	: m_Settings(settings)
{
	const int32 count = m_Mappings.GetCount();
	const float interval = 1.f / (count - 1.f);

	core::VariantMatch(m_Settings,
		[&](const Bezier3& data)
		{
			Vector3f prevPosition = data.m_PointA;
			Vector3f nextPosition = data.m_PointA;

			float distance = 0.f, interpolate = 0.f;
			for (int32 i = 0; i < count; ++i)
			{
				prevPosition = nextPosition;
				nextPosition = Bezier(data.m_PointA, data.m_Control, data.m_PointB, interpolate);

				distance += math::Distance(prevPosition, nextPosition);
				m_Mappings[i] = distance;

				interpolate += interval;
			}
		},
		[&](const Bezier4& data)
		{
			Vector3f prevPosition = data.m_PointA;
			Vector3f nextPosition = data.m_PointA;

			float distance = 0.f, interpolate = 0.f;
			for (int32 i = 0; i < count; ++i)
			{
				prevPosition = nextPosition;
				nextPosition = Bezier(data.m_PointA, data.m_ControlA, data.m_ControlB, data.m_PointB, interpolate);

				distance += math::Distance(prevPosition, nextPosition);
				m_Mappings[i] = distance;

				interpolate += interval;
			}
		},
		[&](const Points& data)
		{
			if (data.m_Positions.IsEmpty())
				return;

			const float distanceTotal = data.m_Distances.GetLast();
			const float distanceNode = distanceTotal / static_cast<float>(count - 1);
			for (int32 i = 0; i < count; ++i)
				m_Mappings[i] = distanceNode * static_cast<float>(i);
		});
}

Vector3f path::Trajectory::AtDistance(const float distance) const
{
	Vector3f result;
	core::VariantMatch(m_Settings,
		[&](const Bezier3& data)
		{
			const float i = ::ToInterpolatedValue(distance, m_Mappings);
			const float t = math::Clamp(i, 0.f, 1.f);
			result = ::Bezier(data.m_PointA, data.m_Control, data.m_PointB, t);
		},
		[&](const Bezier4& data)
		{
			const float i = ::ToInterpolatedValue(distance, m_Mappings);
			const float t = math::Clamp(i, 0.f, 1.f);
			result = ::Bezier(data.m_PointA, data.m_ControlA, data.m_ControlB, data.m_PointB, t);
		},
		[&](const Points& data)
		{
			result = ::ToPosition(data, distance);
		});

	return result;
}

Vector3f path::Trajectory::AtInterpolation(const float interpolation) const
{
	Vector3f result;
	core::VariantMatch(m_Settings,
		[&](const Bezier3& data)
		{
			const float t = math::Clamp(interpolation, 0.f, 1.f);
			result = ::Bezier(data.m_PointA, data.m_Control, data.m_PointB, t);
		},
		[&](const Bezier4& data)
		{
			const float t = math::Clamp(interpolation, 0.f, 1.f);
			result = ::Bezier(data.m_PointA, data.m_ControlA, data.m_ControlB, data.m_PointB, t);
		},
		[&](const Points& data)
		{
			const float d = interpolation * m_Mappings.GetLast();
			result = ::ToPosition(data, d);
		});

	return result;
}

Vector3f path::Trajectory::AtPercentage(const float percentage) const
{
	const float distance = percentage * GetLength();
	return AtDistance(distance);
}
