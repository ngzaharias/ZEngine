#include "GameClientPCH.h"
#include "GameClient/HiddenTrackerSystem.h"

#include <ECS/EntityWorld.h>
#include <ECS/QueryTypes.h>
#include <ECS/WorldView.h>

#include <Engine/LevelComponents.h>

#include "GameClient/HiddenObjectComponents.h"

void hidden::TrackerSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	using Query = ecs::query
		::Added<const hidden::RevealedComponent>
		::Include<const hidden::ObjectComponent>;

	for (const ecs::Entity& entity : world.Query<Query>())
	{
		using Remaining = ecs::query
			::Include<const hidden::ObjectComponent>
			::Exclude<const hidden::RevealedComponent>;
		const auto& query = world.Query<Remaining>();
		if (query.IsEmpty())
		{
			// #todo: load next level
			auto& requestComponent = world.AddEventComponent<eng::level::UnloadRequestComponent>();
			requestComponent.m_Name = NAME("Page01");
		}
	}
}