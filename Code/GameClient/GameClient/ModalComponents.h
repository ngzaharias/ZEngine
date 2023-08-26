#pragma once

#include <ECS/Component.h>

namespace gui::modal
{
	struct MessageComponent : ecs::Component<MessageComponent>
	{
		str::String m_Title = { };
		str::String m_Message = { };
	};
}