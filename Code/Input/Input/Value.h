#pragma once

namespace input
{
	struct Value
	{
		bool IsPressed() const { return m_Pressed; }
		bool IsReleased() const { return m_Released; }
		operator float() const { return m_Value; }

		float m_Value = 0.f;
		bool m_Pressed = false;
		bool m_Released = false;
	};
}