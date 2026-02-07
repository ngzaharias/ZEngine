#pragma once

#include "Core/Guid.h"
#include "Core/String.h"
#include "ECS/Component.h"
#include "ECS/IsPrototype.h"

namespace eng
{
	struct TextComponent final : public ecs::Component<TextComponent>
		, ecs::IsPrototype
	{
		str::String m_Text = { };
		str::Guid m_Font = { };
	};
}