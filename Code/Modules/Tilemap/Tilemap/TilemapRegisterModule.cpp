#include "TilemapPCH.h"
#include "Tilemap/TilemapRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/TemplateManager.h"
#include "Tilemap/TilemapAgentComponent.h"
#include "Tilemap/TilemapAgentTemplate.h"
#include "Tilemap/TilemapDebugSystem.h"
#include "Tilemap/TilemapGridComponent.h"
#include "Tilemap/TilemapGridTemplate.h"

void tilemap::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<tilemap::AgentComponent>();
	world.RegisterComponent<tilemap::AgentTemplate>();
	world.RegisterComponent<tilemap::GridComponent>();
	world.RegisterComponent<tilemap::GridTemplate>();
	world.RegisterSystem<tilemap::DebugSystem>();

	// templates
	{
		auto& manager = world.WriteResource<eng::TemplateManager>();
		manager.RegisterComponent<tilemap::AgentTemplate>();
		manager.RegisterComponent<tilemap::GridTemplate>();
	}
}