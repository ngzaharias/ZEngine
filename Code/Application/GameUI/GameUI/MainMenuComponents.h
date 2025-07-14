#pragma once

#include "Core/Name.h"
#include "ECS/Component.h"

namespace gui::main_menu
{
	struct WindowComponent final : public ecs::Component<WindowComponent>
	{
		str::Name m_NewGame = {};
	};

	struct ContinueGameRequest final : public ecs::EventComponent<ContinueGameRequest> { };
	struct ExitGameRequest final : public ecs::EventComponent<ExitGameRequest> { };
	struct LoadGameRequest final : public ecs::EventComponent<LoadGameRequest> { };
	struct NewGameRequest final : public ecs::EventComponent<NewGameRequest> 
	{ 
		str::Name m_Level = {};
	};
	struct SettingsRequest final : public ecs::EventComponent<SettingsRequest> { };
}
