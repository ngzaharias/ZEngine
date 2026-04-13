#pragma once

#include "ECS/Entity.h"
#include "ECS/Component.h"

namespace editor::entity
{
	struct SelectComponent final : public ecs::StaticComponent
	{
		ecs::Entity m_Entity = {};
	};
}