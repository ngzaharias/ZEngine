#pragma once

#include "Core/Array.h"
#include "ECS/Component.h"
#include "GameUI/InputBindingsCommandEnum.h"

namespace gui::input
{
	struct BindingsComponent final : public ecs::Component
	{
		Array<ECommand> m_Commands = {};
	};
}