#pragma once

#include "Core/String.h"
#include "ECS/Component.h"

namespace gui::modal
{
	struct MessageComponent final : public ecs::Component
	{
		str::String m_Title = { };
		str::String m_Message = { };
	};
}