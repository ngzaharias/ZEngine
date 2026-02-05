#pragma once

#include "Core/Array.h"
#include "ECS/Component.h"
#include "ECS/IsPrototype.h"

namespace gui::input
{
	enum class ECommand
	{
		None,
		GameMenu,
	};

	struct BindingsComponent final : public ecs::Component<BindingsComponent>
		, ecs::IsPrototype
	{
		Array<ECommand> m_Commands = {};
	};
}