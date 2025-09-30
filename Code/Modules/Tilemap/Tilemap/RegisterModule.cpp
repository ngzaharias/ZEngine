#include "TilemapPCH.h"
#include "Tilemap/RegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Tilemap/TilemapAgentComponent.h"
#include "Tilemap/TilemapDebugSystem.h"

void tilemap::RegisterModule(ecs::EntityWorld& entityWorld)
{
	// components
	{
		entityWorld.RegisterComponent<tilemap::AgentComponent>();
	}

	// systems
	{
		entityWorld.RegisterSystem<tilemap::DebugSystem>();
	}
}