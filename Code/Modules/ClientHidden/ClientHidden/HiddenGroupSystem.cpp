#include "ClientHiddenPCH.h"
#include "ClientHidden/HiddenGroupSystem.h"

#include "ClientHidden/HiddenGroupComponent.h"
#include "ClientHidden/HiddenGroupTemplate.h"
#include "ClientHidden/HiddenObjectComponent.h"
#include "ClientHidden/HiddenRevealComponent.h"
#include "Core/Algorithms.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/UUIDComponent.h"

void client::hidden::GroupSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	using GroupQuery = ecs::query
		::Include<client::hidden::GroupComponent, const client::hidden::GroupTemplate>;
	using RevealedQuery = ecs::query
		::Added<const client::hidden::RevealComponent>
		::Include<const eng::UUIDComponent>;

	for (auto&& objectView : world.Query<RevealedQuery>())
	{
		const auto& uuidComponent = objectView.ReadRequired<eng::UUIDComponent>();
		for (auto&& groupView : world.Query<GroupQuery>())
		{
			const auto& groupTemplate = groupView.ReadRequired<client::hidden::GroupTemplate>();
			if (enumerate::Contains(groupTemplate.m_Objects, uuidComponent.m_UUID))
			{
				auto& groupComponent = groupView.WriteRequired<client::hidden::GroupComponent>();
				if (!enumerate::Contains(groupComponent.m_Revealed, uuidComponent.m_UUID))
					groupComponent.m_Revealed.Append(uuidComponent.m_UUID);
			}
		}
	}

	for (auto&& view : world.Query<ecs::query::Added<const client::hidden::GroupTemplate>>())
	{
		world.AddComponent<client::hidden::GroupComponent>(view);
	}

	for (auto&& view : world.Query<ecs::query::Removed<const client::hidden::GroupTemplate>>())
	{
		world.RemoveComponent<client::hidden::GroupComponent>(view);
	}
}