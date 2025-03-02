#pragma once

#include "Core/Array.h"
#include "Core/Input.h"
#include "Core/Variant.h"
#include "Core/String.h"
#include "ECS/Component.h"

namespace gui::input
{
	enum class ECommand
	{
		None,
		ToggleGameMenu,
	};

	using Input = Variant<
		::input::EKeyboard,
		::input::EMouse>;

	struct Binding
	{
		ECommand m_Command = ECommand::None;
		Input m_Input = ::input::EKeyboard::Unknown;
	};

	struct BindingsComponent final : public ecs::Component<BindingsComponent>
	{
		Array<Binding> m_Bindings = {};
	};
}