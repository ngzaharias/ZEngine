#pragma once

#include "Core/Name.h"
#include "ECS/Event.h"

namespace gui::main_menu
{
	struct NewGameEvent final : public ecs::Event 
	{ 
		str::Name m_Level = {};
	};
}
