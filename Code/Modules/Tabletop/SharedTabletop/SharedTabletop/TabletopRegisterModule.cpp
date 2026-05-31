#include "TabletopPCH.h"
#include "SharedTabletop/TabletopRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/TemplateManager.h"
#include "SharedTabletop/TabletopCardComponent.h"
#include "SharedTabletop/TabletopCardTemplate.h"
#include "SharedTabletop/TabletopDeckComponent.h"
#include "SharedTabletop/TabletopDeckTemplate.h"

void shared::tabletop::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<shared::tabletop::CardComponent>();
	world.RegisterComponent<shared::tabletop::CardTemplate>();
	world.RegisterComponent<shared::tabletop::DeckComponent>();
	world.RegisterComponent<shared::tabletop::DeckTemplate>();

	// templates
	{
		auto& manager = world.WriteResource<eng::TemplateManager>();
		manager.RegisterComponent<shared::tabletop::CardTemplate>();
		manager.RegisterComponent<shared::tabletop::DeckTemplate>();
	}
}