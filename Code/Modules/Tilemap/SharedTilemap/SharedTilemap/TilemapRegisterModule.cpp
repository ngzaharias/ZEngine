#include "TilemapPCH.h"
#include "SharedTilemap/TilemapRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/TemplateManager.h"
#include "SharedTilemap/TilemapAgentComponent.h"
#include "SharedTilemap/TilemapAgentTemplate.h"
#include "SharedTilemap/TilemapDebugSystem.h"
#include "SharedTilemap/TilemapGridComponent.h"
#include "SharedTilemap/TilemapGridTemplate.h"

void shared::tilemap::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<shared::tilemap::AgentComponent>();
	world.RegisterComponent<shared::tilemap::AgentTemplate>();
	world.RegisterComponent<shared::tilemap::GridComponent>();
	world.RegisterComponent<shared::tilemap::GridTemplate>();
	world.RegisterSystem<shared::tilemap::DebugSystem>();

	// templates
	{
		auto& manager = world.WriteResource<eng::TemplateManager>();
		manager.RegisterComponent<shared::tilemap::AgentTemplate>();
		manager.RegisterComponent<shared::tilemap::GridTemplate>();
	}
}