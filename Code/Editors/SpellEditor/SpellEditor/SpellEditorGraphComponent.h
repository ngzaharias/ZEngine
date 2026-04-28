#pragma once

#include "ECS/Component.h"
#include "NodeGraph/Graph.h"

namespace editor::spell
{
	struct GraphComponent final : public ecs::Component 
	{ 
		ngraph::Graph m_Graph = {};
	};
};