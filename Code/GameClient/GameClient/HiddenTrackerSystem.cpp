#include "GameClientPCH.h"
#include "GameClient/HiddenTrackerSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/PrototypeManager.h"
#include "Engine/SpriteComponent.h"
#include "GameClient/HiddenObjectComponents.h"
#include "GameClient/ModalComponents.h"

void hidden::TrackerSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	// object revealed
	{
		using GroupQuery = ecs::query::Include<const hidden::GroupComponent>;
		using RevealedQuery = ecs::query
			::Added<const hidden::RevealedComponent>
			::Include<const eng::PrototypeComponent>;

		for (const ecs::Entity& objectEntity : world.Query<RevealedQuery>())
		{
			const auto& objectComponent = world.ReadComponent< eng::PrototypeComponent>(objectEntity);
			for (const ecs::Entity& groupEntity : world.Query<GroupQuery>())
			{
				const auto& groupComponent = world.ReadComponent< hidden::GroupComponent>(groupEntity);
				if (groupComponent.m_Objects.Contains(objectComponent.m_Guid))
				{
					auto& component = world.WriteComponent<hidden::GroupComponent>(groupEntity);
					component.m_Revealed.Add(objectComponent.m_Guid);
				}
			}
		}
	}
	
	// group complete
	{
		using Query = ecs::query::Updated<const hidden::GroupComponent>;
		for (const ecs::Entity& groupEntity : world.Query<Query>())
		{
			const auto& groupComponent = world.ReadComponent< hidden::GroupComponent>(groupEntity);
			if (groupComponent.m_Objects.GetCount() == groupComponent.m_Revealed.GetCount())
			{
				auto& spriteComponent = world.AddComponent<eng::SpriteComponent>(groupEntity);
				spriteComponent.m_Size = groupComponent.m_Size;
				spriteComponent.m_Sprite = groupComponent.m_Sprite;
			}
		}
	}

	// level complete
	{
		using RevealedQuery = ecs::query
			::Added<const hidden::RevealedComponent>
			::Include<const hidden::ObjectComponent>;
		using RemainingQuery = ecs::query
			::Include<const hidden::ObjectComponent>
			::Exclude<const hidden::RevealedComponent>;
		for (const ecs::Entity& objectEntity : world.Query<RevealedQuery>())
		{
			const auto& query = world.Query<RemainingQuery>();
			if (query.IsEmpty())
			{
				// #todo: load next level
				const ecs::Entity messageEntity = world.CreateEntity();
				auto& component = world.AddComponent<gui::modal::MessageComponent>(messageEntity);
				component.m_Message = "Level Complete!";
			}
		}
	}
}