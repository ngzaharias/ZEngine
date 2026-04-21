#pragma once

#include "Core/Guid.h"
#include "Core/Map.h"
#include "Core/String.h"
#include "ECS/Component.h"
#include "Spellcraft/SpellcraftBookComponent.h"

namespace editor::spell
{
	struct Link
	{
		int32 m_SourceId = 0;
		int32 m_TargetId = 0;
	};

	struct Param
	{
		int32 m_NodeId = 0;
	};

	struct Node
	{
		str::String m_Label = {};
		str::Guid m_UUID = {};

		int32 m_InputId = 0;
		int32 m_OutputId = 0;
	};

	struct WindowComponent final : public ecs::Component 
	{ 
		int32 m_Identifier = 0;
		str::String m_Label = {};

		int32 m_GraphIds = 1;
		Map<int32, Link> m_Links = {};
		Map<int32, Node> m_Nodes = {};
		Map<int32, Param> m_Param = {};
	};
};