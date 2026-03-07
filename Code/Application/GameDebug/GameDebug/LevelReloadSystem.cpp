#include "GameDebugPCH.h"
#include "GameDebug/LevelReloadSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/LevelLoadedComponent.h"
#include "Engine/LevelLoadEvent.h"
#include "GameDebug/DebugLevelReloadEvent.h"

void debug::level::ReloadSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<debug::level::ReloadEvent>())
	{
		for (auto&& view : world.Query<ecs::query::Include<const eng::level::LoadedComponent>>())
		{
			const auto& levelComponent = view.ReadRequired<eng::level::LoadedComponent>();
			world.AddEvent<eng::level::LoadEvent>(levelComponent.m_Name);
		}
	}
}
