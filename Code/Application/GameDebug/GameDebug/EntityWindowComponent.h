#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"

namespace debug
{
	struct EntityWindowComponent final : public ecs::Component<EntityWindowComponent> 
	{ 
		int32 m_Identifier = 0;

		str::String m_DockspaceLabel = {};
		str::String m_EntitiesLabel = {};
		str::String m_ComponentsLabel = {};

		ecs::Entity m_ClientEntity = { };
		ecs::Entity m_ServerEntity = { };
	};
};