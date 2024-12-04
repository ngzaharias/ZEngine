#pragma once

#include "Core/Types.h"
#include "Math/Vector.h"

namespace path
{
	/// \brief Object that you allows you to iterate over every cell that a 3d line intersects.
	class Raytracer final
	{
	public:
		Raytracer() = default;
		Raytracer(const Vector3f& source, const Vector3f& target, const float cellSize);

		struct Iterator
		{
			friend class Raytracer;

			auto operator*()->Vector3i;
			auto operator++()->Iterator&;
			bool operator!=(const Iterator& rhs) const;

		private:
			Vector3i m_GridPos = { };
			Vector3i m_Step = Vector3i::Zero;
			Vector3f m_tDelta = Vector3f::Zero;
			Vector3f m_tMax = Vector3f::Zero;

			int32 m_Index = 0;
		};

	public:
		auto begin() const -> Iterator;
		auto end() const -> Iterator;

	public:
		Vector3f m_Source = Vector3f::Zero;
		Vector3f m_Target = Vector3f::Zero;
		float m_CellSize = 0.f;
	};
}