#pragma once

#include "Core/Name.h"
#include "ECS/Component.h"
#include "ECS/Event.h"

namespace gui::main_menu
{
	struct WindowComponent final : public ecs::Component<WindowComponent>
	{
		str::Name m_NewGame = {};
	};

	struct ContinueGameRequest final : public ecs::Event<ContinueGameRequest> { };
	struct ExitGameRequest final : public ecs::Event<ExitGameRequest> { };
	struct LoadGameRequest final : public ecs::Event<LoadGameRequest> { };
	struct NewGameRequest final : public ecs::Event<NewGameRequest> 
	{ 
		str::Name m_Level = {};
	};
}
