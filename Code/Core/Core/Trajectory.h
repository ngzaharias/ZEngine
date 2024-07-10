#pragma once

#include "Core/Array.h"
#include "Core/StaticArray.h"
#include "Core/Variant.h"
#include "Math/Bezier3.h"
#include "Math/Bezier4.h"

namespace path
{
	// #todo: move constructor
	// #todo: move operator
	struct Points
	{
		Array<Vector3f> m_Positions = { };
		Array<float> m_Distances = { };
	};

	/// \brief
	class Trajectory final
	{
	public:
		using Mappings = StaticArray<float, 10>;
		using Settings = Variant<Bezier3, Bezier4, Points>;

	public:
		Trajectory();
		Trajectory(const Settings& settings);

		float GetLength() const;

		/// \brief Sample a position along the trajectory at a distance [0 to Length].
		Vector3f AtDistance(const float distance) const;

		/// \brief Sample a position along the trajectory at an interpolation [0 to 1].
		/// 
		/// This is useful to use for rendering as the distance between two segments 
		/// is relative to the curve. The steeper the curve, the closer the points are.
		Vector3f AtInterpolation(const float interpolation) const;

		/// \brief Sample a position along the trajectory at a percentage [0 to 1].
		Vector3f AtPercentage(const float percentage) const;

	private:
		// used for converting between interpolation values and distances
		Mappings m_Mappings = { };
		Settings m_Settings = { };
	};
}