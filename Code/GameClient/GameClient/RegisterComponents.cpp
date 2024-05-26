#include "GameClientPCH.h"
#include "GameClient/RegisterComponents.h"

#include <ECS/EntityWorld.h>

#include "GameClient/DragComponents.h"
#include "GameClient/GameStateComponents.h"
#include "GameClient/HiddenObjectComponents.h"
#include "GameClient/ModalComponents.h"
#include "GameClient/MovementComponents.h"
#include "GameClient/ProjectileComponents.h"

void clt::RegisterComponents(ecs::EntityWorld& entityWorld)
{
	entityWorld.RegisterComponent<drag::IsSelectableComponent>();
	entityWorld.RegisterComponent<drag::SelectionComponent>();
	entityWorld.RegisterComponent<gamestate::NetworkHostComponent>();
	entityWorld.RegisterComponent<gamestate::NetworkJoinComponent>();
	entityWorld.RegisterComponent<gamestate::NetworkStopComponent>();
	entityWorld.RegisterComponent<gamestate::RequestComponent>();
	entityWorld.RegisterComponent<gamestate::StateComponent>();
	entityWorld.RegisterComponent<gamestate::StateFinishedComponent>();
	entityWorld.RegisterComponent<gui::modal::MessageComponent>();
	entityWorld.RegisterComponent<hidden::ObjectComponent>();
	entityWorld.RegisterComponent<hidden::RevealedComponent>();
	entityWorld.RegisterComponent<movement::AccelerationComponent>();
	entityWorld.RegisterComponent<movement::VelocityComponent>();
	entityWorld.RegisterComponent<projectile::ChangesComponent>();
	entityWorld.RegisterComponent<projectile::CreateRequestComponent>();
	entityWorld.RegisterComponent<projectile::CreateResultComponent>();
	entityWorld.RegisterComponent<projectile::SpawnComponent>();
	entityWorld.RegisterComponent<projectile::TrajectoryComponent>();
}
