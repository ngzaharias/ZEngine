#include "GameClientPCH.h"
#include "GameClient/HiddenCountSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameClient/HiddenCountComponent.h"
#include "GameClient/HiddenGroupComponent.h"

void hidden::CountSystem::Update(World& world, const GameTime& gameTime)
{
	bool hasChanged = false;
	hasChanged |= world.HasAny<ecs::query::Added<hidden::CountComponent>>();
	hasChanged |= world.HasAny<ecs::query::Added<hidden::GroupComponent>>();
	hasChanged |= world.HasAny<ecs::query::Updated<hidden::GroupComponent>>();
	hasChanged |= world.HasAny<ecs::query::Removed<hidden::GroupComponent>>();
	if (hasChanged)
	{
		int32 objects = 0;
		int32 revealed = 0;
		for (const ecs::Entity& entity : world.Query<ecs::query::Include<const hidden::GroupComponent>>())
		{
			const auto& group = world.ReadComponent<hidden::GroupComponent>(entity);
			objects += group.m_Objects.GetCount();
			revealed += group.m_Revealed.GetCount();
		}

		for (const ecs::Entity& entity : world.Query<ecs::query::Include<hidden::CountComponent>>())
		{
			auto& count = world.WriteComponent<hidden::CountComponent>(entity);
			count.m_Objects = objects;
			count.m_Revealed = revealed;
		}
	}
}

#undef SETVALUE