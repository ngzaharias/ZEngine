#pragma once

#include "Core/Name.h"
#include "ECS/Component.h"

namespace gui::main_menu
{
	struct MenuComponent final : public ecs::Component<MenuComponent>
	{
		str::Name m_NewGame = {};
	};
}