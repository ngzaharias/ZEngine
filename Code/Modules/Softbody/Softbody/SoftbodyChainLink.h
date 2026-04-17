#pragma once

#include "Core/Array.h"
#include "ECS/Component.h"
#include "Math/Vector.h"

namespace softbody
{
	struct ChainLink
	{
		Vector2f m_Direction = Vector2f::AxisY;
		Vector2f m_Position = Vector2f::Zero;
	};
}