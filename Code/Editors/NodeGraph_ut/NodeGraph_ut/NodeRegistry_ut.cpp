#include "NodeGraphPCH.h"

#include "Core/Path.h"
#include "Core/StringView.h"
#include "Core/TypeMeta.h"
#include "Engine/FileHelpers.h"
#include "NodeGraph/Graph.h"
#include "NodeGraph/NodeRegistry.h"

#define CLASS_TEST_CASE(name) TEST_CASE("ngraph::NodeRegistry. " name, "[ngraph::NodeRegistry]")

namespace
{
	struct DataWrapper
	{
		DataWrapper()
		{
			const str::Path configDirectory = eng::GetConfigDirectory();
			str::SetPath(str::EPath::Config, configDirectory);
		}
	};
}

CLASS_TEST_CASE("Test.")
{
	DataWrapper data;
	ngraph::Graph graph;
	graph.Initialise();

	const ngraph::NodeRegistry& registry = graph.GetRegistry();
	for (auto&& [name, definition] : registry.GetDefinitions())
	{
		graph.CreateNode(name);
	}
}