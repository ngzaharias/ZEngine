#include "TabletopPCH.h"
#include "Tabletop/TabletopRegisterModule.h"

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
		auto& manager = world.WriteResource<eng::PrototypeManager>();
		manager.RegisterComponent<tabletop::CardComponent>();
		manager.RegisterComponent<tabletop::DeckComponent>();
	}
}