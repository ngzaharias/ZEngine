#pragma once

#include "ECS/Component.h"

namespace gui::modal
{
	struct MessageComponent final : public ecs::Component<MessageComponent>
	{
		str::String m_Title = { };
		str::String m_Message = { };
	};
}