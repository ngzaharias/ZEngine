#pragma once

#include "Core/Name.h"
#include "ECS/Component.h"
#include "ECS/IsPrototype.h"

namespace gui::main_menu
{
	struct WindowComponent final : public ecs::Component
		, ecs::IsPrototype
	{
		str::Name m_NewGame = {};
	};
}
