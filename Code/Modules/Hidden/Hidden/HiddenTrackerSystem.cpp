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

		for (const ecs::Entity& objectEntity : world.Query<RevealedQuery>())
		{
			const auto& objectComponent = world.ReadComponent<eng::PrototypeComponent>(objectEntity);
			for (const ecs::Entity& groupEntity : world.Query<GroupQuery>())
			{
				const auto& groupComponent = world.ReadComponent<hidden::GroupComponent>(groupEntity);
				if (enumerate::Contains(groupComponent.m_Objects, objectComponent.m_Guid))
				{
					auto& component = world.WriteComponent<hidden::GroupComponent>(groupEntity);
					if (!enumerate::Contains(component.m_Revealed, objectComponent.m_Guid))
						component.m_Revealed.Append(objectComponent.m_Guid);
				}
			}
		}
	}
	
	// group complete
	{
		using Query = ecs::query
			::Updated<hidden::GroupComponent>
			::Exclude<eng::SpriteComponent>;
		for (const ecs::Entity& entity : world.Query<Query>())
		{
			const auto& groupComponent = world.ReadComponent<hidden::GroupComponent>(entity);
			if (groupComponent.m_Objects.GetCount() == groupComponent.m_Revealed.GetCount())
			{
				auto& spriteComponent = world.AddComponent<eng::SpriteComponent>(entity);
				spriteComponent.m_Size = groupComponent.m_Size;
				spriteComponent.m_Sprite = groupComponent.m_Sprite;
			}
		}
	}
}