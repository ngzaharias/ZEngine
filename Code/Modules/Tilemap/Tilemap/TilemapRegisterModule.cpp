#include "TilemapPCH.h"
#include "Tilemap/TilemapRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/PrototypeManager.h"
#include "Tilemap/TilemapAgentComponent.h"
#include "Tilemap/TilemapGridComponent.h"
#include "Tilemap/TilemapDebugSystem.h"

void tilemap::RegisterModule(ecs::EntityWorld& world)
{
	// components
	{
		world.RegisterComponent<tilemap::AgentComponent>();
		world.RegisterComponent<tilemap::GridComponent>();
	}

	// systems
	{
		world.RegisterSystem<tilemap::DebugSystem>();
	}

	// prototypes
	{
		auto& manager = world.WriteResource<eng::PrototypeManager>();
		manager.Register<tilemap::AgentComponent>();
		manager.Register<tilemap::GridComponent>();
	}
}