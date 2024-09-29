#include "GameClientPCH.h"
#include "GameClient/HiddenTrackerSystem.h"

#include "Core/Algorithms.h"
#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/PrototypeManager.h"
#include "Engine/SpriteComponent.h"
#include "GameClient/HiddenGroupComponent.h"
#include "GameClient/HiddenObjectComponent.h"
#include "GameClient/HiddenRevealComponent.h"
#include "GameClient/ModalComponents.h"

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
					component.m_Revealed.Add(objectComponent.m_Guid);
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
			if (!world.IsAlive(entity))
				continue;
			if (!world.HasComponent<hidden::GroupComponent>(entity))
				continue;

			const auto& groupComponent = world.ReadComponent<hidden::GroupComponent>(entity);
			if (groupComponent.m_Objects.GetCount() == groupComponent.m_Revealed.GetCount())
			{
				auto& spriteComponent = world.AddComponent<eng::SpriteComponent>(entity);
				spriteComponent.m_Size = groupComponent.m_Size;
				spriteComponent.m_Sprite = groupComponent.m_Sprite;
			}
		}
	}

	// level complete
	{
		using RevealedQuery = ecs::query
			::Added<const hidden::RevealComponent>
			::Include<const hidden::ObjectComponent>;
		using RemainingQuery = ecs::query
			::Include<const hidden::ObjectComponent>
			::Exclude<const hidden::RevealComponent>;
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