#pragma once

#include "Math/Vector.h"

namespace eng
{
	struct WindowConfig
	{
		str::String m_Name = {};

		Vector2u m_Resolution = Vector2u::Zero;
		Vector2u m_Position = Vector2u::Zero;
		int32 m_RefreshRate = 0;

		bool m_IsVSyncEnabled = false;
	};
}