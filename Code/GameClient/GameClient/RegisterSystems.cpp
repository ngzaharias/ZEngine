#include "GameClientPCH.h"
#include "GameClient/RegisterSystems.h"

#include <ECS/EntityWorld.h>
#include <ECS/WorldView.h>

#include <Engine/ReplicationHost.h>

#include "GameClient/DragMovementSystem.h"
#include "GameClient/DragSelectionSystem.h"
#include "GameClient/GameStateSystem.h"
#include "GameClient/ModalStateSystem.h"
#include "GameClient/NetworkHostSystem.h"
#include "GameClient/NetworkJoinSystem.h"
#include "GameClient/NetworkStopSystem.h"
#include "GameClient/ProjectileSpawnSystem.h"

void clt::RegisterSystems(ecs::EntityWorld& entityWorld, const clt::SystemDependencies& dependencies)
{
	entityWorld.RegisterSystem<drag::MovementSystem>();
	entityWorld.RegisterSystem<drag::SelectionSystem>();
	entityWorld.RegisterSystem<gamestate::NetworkHostSystem>();
	entityWorld.RegisterSystem<gamestate::NetworkJoinSystem>();
	entityWorld.RegisterSystem<gamestate::NetworkStopSystem>();
	entityWorld.RegisterSystem<gamestate::StateSystem>();
	entityWorld.RegisterSystem<gui::modal::StateSystem>();
	entityWorld.RegisterSystem<projectile::SpawnSystem>();
}