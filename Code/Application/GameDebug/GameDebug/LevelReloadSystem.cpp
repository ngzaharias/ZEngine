#include "GameDebugPCH.h"
#include "GameDebug/LevelReloadSystem.h"

#include "ECS/EntityWorld.h"
#include "ECS/QueryTypes.h"
#include "ECS/WorldView.h"
#include "Engine/LevelLoadedComponent.h"
#include "Engine/LevelLoadRequest.h"
#include "GameDebug/DebugLevelReloadRequest.h"

void dbg::level::ReloadSystem::Update(World& world, const GameTime& gameTime)
{
	PROFILE_FUNCTION();

	if (world.HasAny<dbg::level::ReloadRequest>())
	{
		for (const ecs::Entity& entity : world.Query<ecs::query::Include<const eng::level::LoadedComponent>>())
		{
			const auto& levelComponent = world.ReadComponent<eng::level::LoadedComponent>(entity);
			world.AddEvent<eng::level::LoadRequest>(levelComponent.m_Name);
		}
	}
}
