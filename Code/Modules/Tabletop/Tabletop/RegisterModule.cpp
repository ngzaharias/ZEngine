#include "TabletopPCH.h"
#include "Tabletop/RegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/PrototypeManager.h"
#include "Tabletop/TabletopCardComponent.h"
#include "Tabletop/TabletopDeckComponent.h"

void tabletop::RegisterModule(ecs::EntityWorld& entityWorld)
{
	// components
	{
		entityWorld.RegisterComponent<tabletop::CardComponent>();
		entityWorld.RegisterComponent<tabletop::DeckComponent>();
	}

	// prototypes
	{
		auto& prototypeManager = entityWorld.WriteResource<eng::PrototypeManager>();
		prototypeManager.Register<tabletop::CardComponent>();
		prototypeManager.Register<tabletop::DeckComponent>();
	}
}