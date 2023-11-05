#include "GameClientPCH.h"
#include "GameClient/RegisterComponents.h"

#include <ECS/EntityWorld.h>

#include "GameClient/DragComponents.h"
#include "GameClient/GameStateComponents.h"
#include "GameClient/ModalComponents.h"
#include "GameClient/MovementComponents.h"
#include "GameClient/ProjectileComponents.h"
#include "GameClient/SpellComponents.h"

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
	entityWorld.RegisterComponent<movement::AccelerationComponent>();
	entityWorld.RegisterComponent<movement::VelocityComponent>();
	entityWorld.RegisterComponent<projectile::ChangesComponent>();
	entityWorld.RegisterComponent<projectile::CreateRequestComponent>();
	entityWorld.RegisterComponent<projectile::CreateResultComponent>();
	entityWorld.RegisterComponent<projectile::SpawnComponent>();
	entityWorld.RegisterComponent<projectile::TrajectoryComponent>();
	entityWorld.RegisterComponent<spell::DefinitionMapComponent>();
	entityWorld.RegisterComponent<spell::RootComponent>();
	entityWorld.RegisterComponent<spell::SubComponent>();
}
