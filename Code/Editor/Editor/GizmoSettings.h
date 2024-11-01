#pragma once

namespace editor::settings
{
	struct Gizmos
	{
		struct CoordAxes
		{
			bool m_IsEnabled = true;
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
			bool m_IsEnabled = true;
			bool m_ShowMajor = true;
			bool m_ShowMinor = false;
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