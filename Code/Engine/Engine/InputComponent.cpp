#include "EnginePCH.h"
#include "Engine/InputComponent.h"

bool eng::InputComponent::IsKeyHeld(input::EKeyboard key) const
{
	return m_KeyboardCurrent.Contains(key);
}

bool eng::InputComponent::IsKeyPressed(input::EKeyboard key) const
{
	return !m_KeyboardPrevious.Contains(key)
		&& m_KeyboardCurrent.Contains(key);
}

bool eng::InputComponent::IsKeyReleased(input::EKeyboard key) const
{
	return m_KeyboardPrevious.Contains(key)
		&& !m_KeyboardCurrent.Contains(key);
}

bool eng::InputComponent::IsKeyHeld(input::EMouse key) const
{
	return m_MouseCurrent.Contains(key);
}

bool eng::InputComponent::IsKeyPressed(input::EMouse key) const
{
	return !m_MousePrevious.Contains(key)
		&& m_MouseCurrent.Contains(key);
}

bool eng::InputComponent::IsKeyReleased(input::EMouse key) const
{
	return m_MousePrevious.Contains(key)
		&& !m_MouseCurrent.Contains(key);
}