#pragma once

#include "Core/Map.h"
#include "Core/Name.h"
#include "NodeGraph/NodeDefinition.h"

namespace ngraph
{
	class Graph;
	struct Node;
}

namespace ngraph
{
	class NodeRegistry
	{
		using NodeMap = Map<str::Name, ngraph::NodeDefinition>;

	public:
		void Initialise();

		auto GetDefinitions() const -> const NodeMap&;

		ngraph::Node Create(const str::Name& name);

	private:
		NodeMap m_NodeMap = {};
	};
}
