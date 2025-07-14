#pragma once

#include "Core/Name.h"
#include "ECS/Component.h"

namespace gui
{
	struct MainMenuComponent final : public ecs::Component<MainMenuComponent>
	{
		str::Name m_NewGame = {};
	};
}