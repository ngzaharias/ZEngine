#pragma once

#include "Core/Name.h"
#include "Core/String.h"
#include "ECS/Component.h"

namespace gui::main_menu
{
	struct WindowComponent final : public ecs::Component<WindowComponent>
	{
		str::String m_Label = "Main Menu";
		str::Name m_NewGame = {};
	};
}