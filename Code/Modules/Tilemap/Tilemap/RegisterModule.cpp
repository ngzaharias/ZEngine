#include "TilemapPCH.h"
#include "Tilemap/RegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/PrototypeManager.h"
#include "Tilemap/TilemapAgentComponent.h"
#include "Tilemap/TilemapGridComponent.h"
#include "Tilemap/TilemapDebugSystem.h"

void tilemap::RegisterModule(ecs::EntityWorld& entityWorld)
{
	// components
	{
		entityWorld.RegisterComponent<tilemap::AgentComponent>();
		entityWorld.RegisterComponent<tilemap::GridComponent>();
	}

	// systems
	{
		entityWorld.RegisterSystem<tilemap::DebugSystem>();
	}

	// prototypes
	{
		auto& prototypeManager = entityWorld.WriteResource<eng::PrototypeManager>();
		prototypeManager.Register<tilemap::AgentComponent>();
		prototypeManager.Register<tilemap::GridComponent>();
	}
}