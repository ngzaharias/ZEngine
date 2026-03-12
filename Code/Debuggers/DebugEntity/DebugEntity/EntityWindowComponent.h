#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"

namespace debug::entity
{
	struct WindowComponent final : public ecs::Component<WindowComponent> 
	{ 
		int32 m_Identifier = 0;

		str::String m_DockspaceLabel = {};
		str::String m_EntitiesLabel = {};
		str::String m_ComponentsLabel = {};

		ecs::Entity m_ClientEntity = { };
		ecs::Entity m_ServerEntity = { };
	};
};