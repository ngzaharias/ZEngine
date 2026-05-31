#pragma once

#include "ECS/Component.h"
#include "NodeGraph/Graph.h"

namespace editor::spellcraft
{
	struct GraphComponent final : public ecs::Component 
	{ 
		ngraph::Graph m_Graph = {};
	};
};