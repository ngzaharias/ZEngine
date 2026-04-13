#pragma once

#include "Core/Guid.h"
#include "Core/String.h"
#include "ECS/Component.h"

namespace eng
{
	struct TextTemplate final : public ecs::TemplateComponent
	{
		str::String m_Text = { };
		str::Guid m_Font = { };
	};
}