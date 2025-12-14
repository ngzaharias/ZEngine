#pragma once

#include "Core/String.h"
#include "ECS/Component.h"

namespace editor
{
	struct InputWindowComponent final : public ecs::Component<InputWindowComponent>
	{
		int32 m_Identifier = 0;
		str::String m_Label = {};
	};
}