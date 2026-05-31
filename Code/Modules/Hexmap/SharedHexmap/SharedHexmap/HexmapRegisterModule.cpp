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

void shared::hexmap::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<shared::hexmap::LayerComponent>();
	world.RegisterComponent<shared::hexmap::RootComponent>();
	world.RegisterComponent<shared::hexmap::RootTemplate>();
	world.RegisterSystem<shared::hexmap::LoadSystem>();
	world.RegisterSystem<shared::hexmap::ModifySystem>();
	world.RegisterSystem<shared::hexmap::RenderSystem>();
	world.RegisterSystem<shared::hexmap::RootSystem>();

	// templates
	{
		auto& manager = world.WriteResource<eng::TemplateManager>();
		manager.RegisterComponent<shared::hexmap::RootTemplate>();
	}
}