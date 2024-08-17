#include "GameClientPCH.h"
#include "GameClient/HiddenTrackerSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "GameClient/HiddenObjectComponents.h"
#include "GameClient/ModalComponents.h"

void hidden::TrackerSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	using RevealedQuery = ecs::query
		::Added<const hidden::RevealedComponent>
		::Include<const hidden::ObjectComponent>;

	for (const ecs::Entity& entity : world.Query<RevealedQuery>())
	{
		using RemainingQuery = ecs::query
			::Include<const hidden::ObjectComponent>
			::Exclude<const hidden::RevealedComponent>;
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