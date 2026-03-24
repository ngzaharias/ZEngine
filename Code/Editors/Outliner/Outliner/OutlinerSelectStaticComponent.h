#pragma once

#include "ECS/Entity.h"
#include "ECS/Component.h"

namespace editor::outliner
{
	struct SelectStaticComponent final : public ecs::StaticComponent
	{
		ecs::Entity m_Entity = {};
	};
}