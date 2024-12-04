#pragma once

#include "Math/Vector.h"

namespace editor::settings
{
	struct Gizmos
	{
		struct CoordAxes
		{
			bool m_IsEnabled = false;
			bool m_ShowX = true;
			bool m_ShowY = false;
			bool m_ShowZ = true;
		};

		struct Crosshair
		{
			bool m_IsEnabled = true;
		};

		struct FloorGrid
		{
			bool m_IsEnabled = false;
			bool m_ShowMajor = true;
			bool m_ShowMinor = false;

			float m_MajorScale = 100.f;
			float m_MajorWidth = 0.01f;
			Vector3f m_MajorColour = Vector3f(0.4f);
		};

		struct Transform
		{
			bool m_IsEnabled = true;
		};

		bool m_IsEnabled = true;
		CoordAxes m_CoordAxes = {};
		Crosshair m_Crosshair = {};
		FloorGrid m_FloorGrid = {};
		Transform m_Transform = {};
	};
}