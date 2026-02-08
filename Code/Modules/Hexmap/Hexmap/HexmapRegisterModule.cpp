#include "HexmapPCH.h"
#include "Hexmap/HexmapRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/PrototypeManager.h"
#include "Hexmap/HexmapLayerComponent.h"
#include "Hexmap/HexmapLoadSystem.h"
#include "Hexmap/HexmapModifySystem.h"
#include "Hexmap/HexmapRenderSystem.h"
#include "Hexmap/HexmapRootComponent.h"
#include "Hexmap/HexmapRootSystem.h"

void hexmap::RegisterModule(ecs::EntityWorld& world)
{
	// components
	{
		world.RegisterComponent<hexmap::LayerComponent>();
		world.RegisterComponent<hexmap::RootComponent>();
	}

	// systems
	{
		world.RegisterSystem<hexmap::LoadSystem>();
		world.RegisterSystem<hexmap::ModifySystem>();
		world.RegisterSystem<hexmap::RenderSystem>();
		world.RegisterSystem<hexmap::RootSystem>();
	}

	// prototypes
	{
		auto& manager = world.WriteResource<eng::PrototypeManager>();
		manager.RegisterComponent<hexmap::RootComponent>();
	}
}