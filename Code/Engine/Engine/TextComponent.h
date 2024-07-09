#pragma once

#include "Core/Guid.h"
#include "Core/String.h"
#include "Core/Vector.h"
#include "ECS/Component.h"

namespace eng
{
	struct TextComponent : ecs::Component<TextComponent>
	{
		str::String m_Text = { };
		str::Guid m_Font = { };
	};
}