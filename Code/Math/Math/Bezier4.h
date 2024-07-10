#pragma once

#include "Math/Vector.h"

struct Bezier4
{
	Vector3f m_ControlA = Vector3f::Zero;
	Vector3f m_ControlB = Vector3f::Zero;
	Vector3f m_PointA = Vector3f::Zero;
	Vector3f m_PointB = Vector3f::Zero;
};