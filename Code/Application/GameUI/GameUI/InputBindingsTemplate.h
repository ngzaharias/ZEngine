#pragma once

#include "Core/Array.h"
#include "ECS/Component.h"
#include "GameUI/InputBindingsCommandEnum.h"

namespace gui::input
{
	struct BindingsTemplate final : public ecs::TemplateComponent
	{
		Array<ECommand> m_Commands = {};
	};
}