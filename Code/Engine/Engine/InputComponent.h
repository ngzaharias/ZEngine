#pragma once

#include "Core/Input.h"
#include "Core/Set.h"
#include "ECS/Component.h"
#include "Math/Vector.h"

namespace eng
{
	struct InputComponent : public ecs::Component<InputComponent>
	{
		bool IsKeyHeld(input::EKeyboard key) const;
		bool IsKeyPressed(input::EKeyboard key) const;
		bool IsKeyReleased(input::EKeyboard key) const;

		bool IsKeyHeld(input::EMouse key) const;
		bool IsKeyPressed(input::EMouse key) const;
		bool IsKeyReleased(input::EMouse key) const;

		Set<input::EKeyboard> m_KeyboardPrevious;
		Set<input::EKeyboard> m_KeyboardCurrent;
		Set<input::EMouse> m_MousePrevious;
		Set<input::EMouse> m_MouseCurrent;
		Vector2f m_MouseDelta;
		Vector2f m_MousePosition;
		Vector2f m_ScrollDelta;
	};
}