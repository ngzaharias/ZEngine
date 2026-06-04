#include "TilemapPCH.h"
#include "DebugTilemap/TilemapRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "DebugTilemap/TilemapGridSystem.h"

void debug::tilemap::RegisterModule(ecs::EntityWorld& clientWorld, ecs::EntityWorld& serverWorld)
{
	clientWorld.RegisterSystem<debug::tilemap::GridSystem>();
}