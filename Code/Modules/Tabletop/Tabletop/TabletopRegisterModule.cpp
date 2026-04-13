#include "TabletopPCH.h"
#include "Tabletop/TabletopRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/TemplateManager.h"
#include "Tabletop/TabletopCardComponent.h"
#include "Tabletop/TabletopCardTemplate.h"
#include "Tabletop/TabletopDeckComponent.h"
#include "Tabletop/TabletopDeckTemplate.h"

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