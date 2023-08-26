#pragma once

#include <Core/Array.h>

#include <Editor/Node.h>
#include <Editor/NodeVariable.h>

namespace node
{
	using NodeIndex = int32;
	using VarIndex = int32;
	struct Graph
	{
		Array<Var> m_Vars = { };
		Array<Node> m_Nodes = { };
	};
}