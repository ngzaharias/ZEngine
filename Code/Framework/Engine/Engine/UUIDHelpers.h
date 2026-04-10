#pragma once

#include "ECS/WorldView.h"
#include "Engine/UUIDComponent.h"
#include "Engine/UUIDMapComponent.h"

namespace ecs
{
	class EntityWorld;
}

namespace eng
{
	struct UUIDComponent;
	struct UUIDMapComponent;
}

namespace eng
{
	using ToEntityWorld = ecs::WorldView::Read<eng::UUIDMapComponent>;
	ecs::Entity ToEntity(ToEntityWorld& world, const str::Guid& guid);
	ecs::Entity ToEntity(ecs::EntityWorld& world, const str::Guid& guid);

	using ToUUIDWorld = ecs::WorldView::Read<eng::UUIDComponent>;
	str::Guid ToUUID(ToUUIDWorld& world, const ecs::Entity& entity);
	str::Guid ToUUID(ecs::EntityWorld& world, const ecs::Entity& entity);
}
