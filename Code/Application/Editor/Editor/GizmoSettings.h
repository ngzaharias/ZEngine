#pragma once

#include "Core/Colour.h"
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

			Colour m_ColourX = Colour(0.58f, 0.23f, 0.29f);
			Colour m_ColourY = Colour(0.38f, 0.52f, 0.16f);
			Colour m_ColourZ = Colour(0.21f, 0.4f, 0.61f);
		};

		struct Crosshair
		{
			bool m_IsEnabled = true;
		};

		struct FloorGrid
		{
			bool m_IsEnabled = false;
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