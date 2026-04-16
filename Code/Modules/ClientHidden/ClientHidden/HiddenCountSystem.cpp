#include "ClientHiddenPCH.h"
#include "ClientHidden/HiddenCountSystem.h"

#include "ClientHidden/HiddenCountComponent.h"
#include "ClientHidden/HiddenCountTemplate.h"
#include "ClientHidden/HiddenGroupComponent.h"
#include "ClientHidden/HiddenGroupTemplate.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"

void client::hidden::CountSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	bool hasChanged = false;
	hasChanged |= world.HasAny<ecs::query::Added<client::hidden::CountComponent>>();
	hasChanged |= world.HasAny<ecs::query::Added<client::hidden::GroupComponent>>();
	hasChanged |= world.HasAny<ecs::query::Updated<client::hidden::GroupComponent>>();
	hasChanged |= world.HasAny<ecs::query::Removed<client::hidden::GroupComponent>>();
	if (hasChanged)
	{
		int32 objects = 0;
		int32 revealed = 0;
		for (auto&& view : world.Query<ecs::query::Include<const client::hidden::GroupComponent, const client::hidden::GroupTemplate>>())
		{
			const auto& groupComponent = view.ReadRequired<client::hidden::GroupComponent>();
			const auto& groupTemplate = view.ReadRequired<client::hidden::GroupTemplate>();
			objects += groupTemplate.m_Objects.GetCount();
			revealed += groupComponent.m_Revealed.GetCount();
		}

		for (auto&& view : world.Query<ecs::query::Include<client::hidden::CountComponent>>())
		{
			auto& count = view.WriteRequired<client::hidden::CountComponent>();
			count.m_Objects = objects;
			count.m_Revealed = revealed;
		}
	}

	for (auto&& view : world.Query<ecs::query::Added<const client::hidden::CountTemplate>>())
	{
		world.AddComponent<client::hidden::CountComponent>(view);
	}

	for (auto&& view : world.Query<ecs::query::Removed<const client::hidden::CountTemplate>>())
	{
		world.RemoveComponent<client::hidden::CountComponent>(view);
	}
}
