#pragma once

#include "Engine/WindowModeEnum.h"
#include "Math/Vector.h"

namespace eng
{
	struct WindowConfig
	{
		str::String m_Name = "ZEngine";
		Vector2u m_Position = Vector2u::Zero;
		Vector2u m_Resolution = Vector2u::Zero;
		EWindowMode m_Mode = EWindowMode::Fullscreen;
		int32 m_RefreshRate = 0;
	};
}