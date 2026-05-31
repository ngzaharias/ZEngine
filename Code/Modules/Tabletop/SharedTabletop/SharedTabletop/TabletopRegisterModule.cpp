#include "TabletopPCH.h"
#include "SharedTabletop/TabletopRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/TemplateManager.h"
#include "SharedTabletop/TabletopCardComponent.h"
#include "SharedTabletop/TabletopCardTemplate.h"
#include "SharedTabletop/TabletopDeckComponent.h"
#include "SharedTabletop/TabletopDeckTemplate.h"

void tabletop::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<tabletop::CardComponent>();
	world.RegisterComponent<tabletop::CardTemplate>();
	world.RegisterComponent<tabletop::DeckComponent>();
	world.RegisterComponent<tabletop::DeckTemplate>();

	// templates
	{
		auto& manager = world.WriteResource<eng::TemplateManager>();
		manager.RegisterComponent<tabletop::CardTemplate>();
		manager.RegisterComponent<tabletop::DeckTemplate>();
	}
}