#include "TilemapPCH.h"
#include "ServerTilemap/TilemapRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "ServerTilemap/TilemapAgentSystem.h"
#include "ServerTilemap/TilemapGridSystem.h"

void server::tilemap::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterSystem<server::tilemap::AgentSystem>();
	world.RegisterSystem<server::tilemap::GridSystem>();
}