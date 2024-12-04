#include "Core/Raytracer.h"

#include "Math/VectorMath.h"

#include <limits>
#include <utility>

// https://github.com/feelinfine/tracer/blob/master/tracer.h
// http://www.cse.chalmers.se/edu/year/2011/course/TDA361/Advanced%20Computer%20Graphics/grid.pdf
// https://www.flipcode.com/archives/Raytracing_Topics_Techniques-Part_4_Spatial_Subdivisions.shtml

path::Raytracer::Raytracer(const Vector3f& source, const Vector3f& target, const float cellSize)
	: m_Source(source)
	, m_Target(target)
	, m_CellSize(cellSize)
{
}

auto path::Raytracer::Iterator::operator*() -> Vector3i
{
	return m_GridPos;
}

auto path::Raytracer::Iterator::operator++() -> Iterator&
{
	const float absX = std::abs(m_tMax.x);
	const float absY = std::abs(m_tMax.y);
	const float absZ = std::abs(m_tMax.z);
	if (absX < absY && absX < absZ)
	{
		m_tMax.x += m_tDelta.x;
		m_GridPos.x += m_Step.x;
	}
	else if (absY < absZ)
	{
		m_tMax.y += m_tDelta.y;
		m_GridPos.y += m_Step.y;
	}
	else
	{
		m_tMax.z += m_tDelta.z;
		m_GridPos.z += m_Step.z;
	}
	m_Index++;

	return *this;
}

bool path::Raytracer::Iterator::operator!=(const Iterator& rhs) const
{
	return m_Index != rhs.m_Index;
}

auto path::Raytracer::begin() const -> Iterator
{
	const Vector3f direction = (m_Target - m_Source).Normalized();
	const Vector3i gridPosA = math::ToGridPos(m_Source, m_CellSize);
	const Vector3i gridPosB = math::ToGridPos(m_Target, m_CellSize);

	Iterator itr;
	itr.m_Step.x = (direction.x >= 0.f) ? +1 : -1;
	itr.m_Step.y = (direction.y >= 0.f) ? +1 : -1;
	itr.m_Step.z = (direction.z >= 0.f) ? +1 : -1;

	// Distance to the nearest side.
	const float nearX = (itr.m_Step.x >= 0)
		? (gridPosA.x + 1) * m_CellSize - m_Source.x
		: m_Source.x - (gridPosA.x * m_CellSize);
	const float nearY = (itr.m_Step.y >= 0)
		? (gridPosA.y + 1) * m_CellSize - m_Source.y
		: m_Source.y - (gridPosA.y * m_CellSize);
	const float nearZ = (itr.m_Step.z >= 0)
		? (gridPosA.z + 1) * m_CellSize - m_Source.z
		: m_Source.z - (gridPosA.z * m_CellSize);

	// How far along the line we must move to cross a SINGLE vertical or horizontal boundary.
	itr.m_tDelta.x = (direction.x != 0) ? m_CellSize / direction.x : std::numeric_limits<float>::infinity();
	itr.m_tDelta.y = (direction.y != 0) ? m_CellSize / direction.y : std::numeric_limits<float>::infinity();
	itr.m_tDelta.z = (direction.z != 0) ? m_CellSize / direction.z : std::numeric_limits<float>::infinity();

	// How far along the line we must move to cross the FIRST vertical or horizontal boundary.
	itr.m_tMax.x = (direction.x != 0) ? nearX / direction.x : std::numeric_limits<float>::infinity();
	itr.m_tMax.y = (direction.y != 0) ? nearY / direction.y : std::numeric_limits<float>::infinity();
	itr.m_tMax.z = (direction.z != 0) ? nearZ / direction.z : std::numeric_limits<float>::infinity();

	itr.m_GridPos = gridPosA;
	return std::move(itr);
}

auto path::Raytracer::end() const -> Iterator
{
	const Vector3i gridPosA = math::ToGridPos(m_Source, m_CellSize);
	const Vector3i gridPosB = math::ToGridPos(m_Target, m_CellSize);

	Iterator itr;
	itr.m_Index = 
		std::abs(gridPosB.x - gridPosA.x) + 
		std::abs(gridPosB.y - gridPosA.y) + 
		std::abs(gridPosB.z - gridPosA.z) + 1;
	return std::move(itr);
}