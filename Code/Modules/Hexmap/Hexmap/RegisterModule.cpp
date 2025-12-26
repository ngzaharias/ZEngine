#include "HexmapPCH.h"
#include "Hexmap/RegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/PrototypeManager.h"
#include "Hexmap/HexmapLayerComponent.h"
#include "Hexmap/HexmapLoadSystem.h"
#include "Hexmap/HexmapModifySystem.h"
#include "Hexmap/HexmapRenderSystem.h"
#include "Hexmap/HexmapRootComponent.h"
#include "Hexmap/HexmapRootSystem.h"

void hexmap::RegisterModule(ecs::EntityWorld& entityWorld)
{
	// systems
	{
		entityWorld.RegisterSystem<hexmap::LoadSystem>();
		entityWorld.RegisterSystem<hexmap::ModifySystem>();
		entityWorld.RegisterSystem<hexmap::RenderSystem>();
		entityWorld.RegisterSystem<hexmap::RootSystem>();
	}

	// components
	{
		entityWorld.RegisterComponent<hexmap::LayerComponent>();
		entityWorld.RegisterComponent<hexmap::RootComponent>();
	}

	// prototypes
	{
		auto& prototypeManager = entityWorld.WriteResource<eng::PrototypeManager>();
		prototypeManager.Register<hexmap::RootComponent>();
	}
}