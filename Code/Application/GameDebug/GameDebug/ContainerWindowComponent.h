#pragma once

#include "Core/String.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"

namespace debug
{
	struct ContainerWindowComponent final : public ecs::Component<ContainerWindowComponent> 
	{ 
		int32 m_Identifier = 0;
		str::String m_Label = {};
		ecs::Entity m_Storage = { };
	};
};