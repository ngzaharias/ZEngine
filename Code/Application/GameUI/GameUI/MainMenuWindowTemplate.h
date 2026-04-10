#pragma once

#include "Core/Name.h"
#include "ECS/Component.h"

namespace gui::main_menu
{
	struct WindowTemplate final : public ecs::TemplateComponent
	{
		str::Name m_NewGame = {};
	};
}
