#pragma once

#include "ECS/WorldView.h"
#include "Engine/TemplateComponent.h"
#include "Engine/TemplateMapComponent.h"

namespace ecs
{
	class EntityWorld;
}

namespace eng
{
	struct TemplateComponent;
	struct TemplateMapComponent;
}

namespace eng
{
	using ToEntityWorld = ecs::WorldView::Read<eng::TemplateMapComponent>;
	ecs::Entity ToEntity(ToEntityWorld& world, const str::Guid& guid);
	ecs::Entity ToEntity(ecs::EntityWorld& world, const str::Guid& guid);

	using ToGuidWorld = ecs::WorldView::Read<eng::TemplateComponent>;
	str::Guid ToGuid(ToGuidWorld& world, const ecs::Entity& entity);
	str::Guid ToGuid(ecs::EntityWorld& world, const ecs::Entity& entity);
}
