#include "TilemapPCH.h"
#include "Tilemap/RegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Tilemap/TilemapAgentComponent.h"

void tilemap::RegisterModule(ecs::EntityWorld& entityWorld)
{
	// components
	{
		entityWorld.RegisterComponent<tilemap::AgentComponent>();
	}
}