#pragma once

#include "Core/Array.h"
#include "ECS/Component.h"

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