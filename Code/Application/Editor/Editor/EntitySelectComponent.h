#pragma once

#include "ECS/Component.h"
#include "ECS/Entity.h"

namespace editor
{
	struct EntitySelectSingleton : public ecs::Singleton<EntitySelectSingleton>
	{
		ecs::Entity m_Entity = {};
	};
}