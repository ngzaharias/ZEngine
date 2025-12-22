#include "HiddenPCH.h"
#include "Hidden/HiddenTrackerSystem.h"

#include "Core/Algorithms.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/PrototypeManager.h"
#include "Engine/SpriteComponent.h"
#include "Hidden/HiddenGroupComponent.h"
#include "Hidden/HiddenObjectComponent.h"
#include "Hidden/HiddenRevealComponent.h"

void hidden::TrackerSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	// object revealed
	{
		using GroupQuery = ecs::query::Include<const hidden::GroupComponent>;
		using RevealedQuery = ecs::query
			::Added<const hidden::RevealComponent>
			::Include<const eng::PrototypeComponent>;

		for (auto&& objectView : world.Query<RevealedQuery>())
		{
			const auto& objectComponent = objectView.ReadRequired<eng::PrototypeComponent>();
			for (auto&& groupView : world.Query<GroupQuery>())
			{
				const auto& groupComponent = groupView.ReadRequired<hidden::GroupComponent>();
				if (enumerate::Contains(groupComponent.m_Objects, objectComponent.m_Guid))
				{
					auto& component = groupView.WriteRequired<hidden::GroupComponent>();
					if (!enumerate::Contains(component.m_Revealed, objectComponent.m_Guid))
						component.m_Revealed.Append(objectComponent.m_Guid);
				}
			}
		}
	}
}