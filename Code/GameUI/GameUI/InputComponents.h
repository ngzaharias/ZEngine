#pragma once

#include "Core/Array.h"
#include "Core/Variant.h"
#include "Core/String.h"
#include "ECS/Component.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

namespace gui::input
{
	enum class ECommand
	{
		None,
		GameMenu,
	};

	struct BindingsComponent final : public ecs::Component<BindingsComponent>
	{
		Array<ECommand> m_Commands = {};
	};
}