#pragma once

#include "Core/Array.h"
#include "Core/Guid.h"
#include "Core/Map.h"
#include "NodeGraph/Link.h"
#include "NodeGraph/Node.h"
#include "NodeGraph/NodeRegistry.h"

namespace str
{
	class Name;
}

namespace ngraph
{
	using GraphId = int32;

	class Graph
	{
		using GraphMap = Map<GraphId, str::Guid>;
		using FieldMap = Map<str::Guid, ngraph::Field>;
		using LinkMap = Map<str::Guid, ngraph::Link>;
		using NodeMap = Map<str::Guid, ngraph::Node>;

	public:
		void Initialise();

		auto GetField(const GraphId& graphId) const -> const ngraph::Field&;

		auto GetLink(const str::Guid& uuid) const -> const ngraph::Link&;
		auto GetLinks() const -> const LinkMap&;
		
		auto GetNode(const GraphId& graphId) const -> const ngraph::Node&;
		auto GetNode(const str::Guid& uuid) const -> const ngraph::Node&;
		auto GetNodes() const -> const NodeMap&;

		auto GetRegistry() const -> const ngraph::NodeRegistry&;

		//////////////////////////////////////////////////////////////////////////
		// Link
		
		void CreateLink(const GraphId sourceId, const GraphId targetId);
		
		void DestroyLink(const int32 index);

		//////////////////////////////////////////////////////////////////////////
		// Node

		void CreateNode(const str::Name& name);
		
		void DestroyNode(const str::Guid& uuid);

	private:
		GraphId m_GraphId = 1;
		GraphMap m_GraphMap = {};

		FieldMap m_FieldMap = {};
		LinkMap m_LinkMap = {};
		NodeMap m_NodeMap = {};

		ngraph::NodeRegistry m_Registry = {};
	};
}
