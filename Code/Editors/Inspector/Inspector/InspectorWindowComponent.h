#pragma once

#include "Core/String.h"
#include "ECS/Component.h"

namespace editor::inspector
{
	struct WindowComponent final : public ecs::Component<WindowComponent>
	{
		int32 m_Identifier = 0;
		str::String m_Label = {};
	};
}
