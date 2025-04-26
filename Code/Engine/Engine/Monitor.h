#pragma once

#include "Math/Vector.h"

namespace eng
{
	class Monitor
	{
	public:
		Vector2u m_Resolution = Vector2u::Zero;
		Vector2u m_Position = Vector2u::Zero;
		int32 m_RefreshRate = 0;
	};
}