#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "NodeGraph/Graph.h"

namespace editor::spell
{
	struct WindowComponent final : public ecs::Component 
	{ 
		int32 m_Identifier = 0;
		str::String m_Label = {};
	};
};