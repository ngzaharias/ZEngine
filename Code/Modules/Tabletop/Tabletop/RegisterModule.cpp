#include "TabletopPCH.h"
#include "Tabletop/RegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/PrototypeManager.h"
#include "Tabletop/TabletopCardComponent.h"
#include "Tabletop/TabletopDeckComponent.h"

void tabletop::RegisterModule(ecs::EntityWorld& world)
{
	// components
	{
		world.RegisterComponent<tabletop::CardComponent>();
		world.RegisterComponent<tabletop::DeckComponent>();
	}

	// prototypes
	{
		auto& manager = entityWorld.WriteResource<eng::PrototypeManager>();
		manager.Register<tabletop::CardComponent>();
		manager.Register<tabletop::DeckComponent>();
	}
}