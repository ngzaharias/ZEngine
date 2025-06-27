#pragma once

#include "ECS/Component.h"
#include "ECS/Entity.h"

namespace editor
{
	struct EntitySelectComponent : public ecs::SingletonComponent<EntitySelectComponent>
	{
		ecs::Entity m_Entity = {};
	};
}