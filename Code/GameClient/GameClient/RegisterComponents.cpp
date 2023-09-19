#include "GameClientPCH.h"
#include "GameClient/RegisterComponents.h"

#include <ECS/EntityWorld.h>

#include "GameClient/DragComponents.h"
#include "GameClient/GameStateComponents.h"
#include "GameClient/ModalComponents.h"
#include "GameClient/ProjectileComponents.h"

void clt::RegisterComponents(ecs::EntityWorld& entityWorld)
{
	entityWorld.RegisterComponent<drag::SelectionComponent>();
	entityWorld.RegisterComponent<gamestate::NetworkHostComponent>();
	entityWorld.RegisterComponent<gamestate::NetworkJoinComponent>();
	entityWorld.RegisterComponent<gamestate::NetworkStopComponent>();
	entityWorld.RegisterComponent<gamestate::RequestComponent>();
	entityWorld.RegisterComponent<gamestate::StateComponent>();
	entityWorld.RegisterComponent<gamestate::StateFinishedComponent>();
	entityWorld.RegisterComponent<gui::modal::MessageComponent>();
	entityWorld.RegisterComponent<projectile::RequestComponent>();
	entityWorld.RegisterComponent<projectile::SettingsComponent>();
	entityWorld.RegisterComponent<projectile::StateComponent>();
}
