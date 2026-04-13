#include "HexmapPCH.h"
#include "Hexmap/HexmapRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/TemplateManager.h"
#include "Hexmap/HexmapLayerComponent.h"
#include "Hexmap/HexmapLoadSystem.h"
#include "Hexmap/HexmapModifySystem.h"
#include "Hexmap/HexmapRenderSystem.h"
#include "Hexmap/HexmapRootComponent.h"
#include "Hexmap/HexmapRootSystem.h"
#include "Hexmap/HexmapRootTemplate.h"

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