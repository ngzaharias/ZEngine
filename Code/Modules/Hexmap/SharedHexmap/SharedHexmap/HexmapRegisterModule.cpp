#include "HexmapPCH.h"
#include "SharedHexmap/HexmapRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/TemplateManager.h"
#include "SharedHexmap/HexmapLayerComponent.h"
#include "SharedHexmap/HexmapLoadSystem.h"
#include "SharedHexmap/HexmapModifySystem.h"
#include "SharedHexmap/HexmapRenderSystem.h"
#include "SharedHexmap/HexmapRootComponent.h"
#include "SharedHexmap/HexmapRootSystem.h"
#include "SharedHexmap/HexmapRootTemplate.h"

void hexmap::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<hexmap::LayerComponent>();
	world.RegisterComponent<hexmap::RootComponent>();
	world.RegisterComponent<hexmap::RootTemplate>();
	world.RegisterSystem<hexmap::LoadSystem>();
	world.RegisterSystem<hexmap::ModifySystem>();
	world.RegisterSystem<hexmap::RenderSystem>();
	world.RegisterSystem<hexmap::RootSystem>();

	// templates
	{
		auto& manager = world.WriteResource<eng::TemplateManager>();
		manager.RegisterComponent<hexmap::RootTemplate>();
	}
}