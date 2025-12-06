#pragma once

#include "Core/Name.h"
#include "ECS/Event.h"

namespace gui::main_menu
{
	struct NewGameRequest final : public ecs::Event<NewGameRequest> 
	{ 
		str::Name m_Level = {};
	};
}
