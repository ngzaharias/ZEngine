#pragma once

#include "Math/Vector.h"

struct Bezier3
{
	Vector3f m_Control = Vector3f::Zero;
	Vector3f m_PointA = Vector3f::Zero;
	Vector3f m_PointB = Vector3f::Zero;
};