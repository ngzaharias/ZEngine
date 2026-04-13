#include "ClientHiddenPCH.h"
#include "ClientHidden/HiddenTrackerSystem.h"

#include "ClientHidden/HiddenGroupComponent.h"
#include "ClientHidden/HiddenObjectComponent.h"
#include "ClientHidden/HiddenRevealComponent.h"
#include "Core/Algorithms.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/SpriteComponent.h"
#include "Engine/UUIDComponent.h"

void client::hidden::TrackerSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	// object revealed
	{
		using GroupQuery = ecs::query::Include<const client::hidden::GroupComponent>;
		using RevealedQuery = ecs::query
			::Added<const client::hidden::RevealComponent>
			::Include<const eng::UUIDComponent>;

		for (auto&& objectView : world.Query<RevealedQuery>())
		{
			const auto& uuidComponent = objectView.ReadRequired<eng::UUIDComponent>();
			for (auto&& groupView : world.Query<GroupQuery>())
			{
				const auto& groupComponent = groupView.ReadRequired<client::hidden::GroupComponent>();
				if (enumerate::Contains(groupComponent.m_Objects, uuidComponent.m_UUID))
				{
					auto& component = groupView.WriteRequired<client::hidden::GroupComponent>();
					if (!enumerate::Contains(component.m_Revealed, uuidComponent.m_UUID))
						component.m_Revealed.Append(uuidComponent.m_UUID);
				}
			}
		}
	}
}