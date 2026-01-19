#include "GameClientPCH.h"
#include "GameClient/RegisterSystems.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "GameClient/AccelerationSystem.h"
#include "GameClient/DragMovementSystem.h"
#include "GameClient/DragSelectionSystem.h"
#include "GameClient/GameStateSystem.h"
#include "GameClient/ModalStateSystem.h"
#include "GameClient/ProjectileSpawnSystem.h"
#include "GameClient/ProjectileTrajectorySystem.h"
#include "GameClient/SettingsSystem.h"
#include "GameClient/TransformSystem.h"
#include "GameClient/VelocitySystem.h"
#include "GameClient/VisualSystem.h"

void client::RegisterSystems(ecs::EntityWorld& entityWorld)
{
	entityWorld.RegisterSystem<client::SettingsSystem>();
	entityWorld.RegisterSystem<drag::MovementSystem>();
	entityWorld.RegisterSystem<drag::SelectionSystem>();
	entityWorld.RegisterSystem<gamestate::StateSystem>();
	entityWorld.RegisterSystem<gui::modal::StateSystem>();
	entityWorld.RegisterSystem<movement::AccelerationSystem>();
	entityWorld.RegisterSystem<movement::VelocitySystem>();
	entityWorld.RegisterSystem<projectile::SpawnSystem>();
	entityWorld.RegisterSystem<projectile::TrajectorySystem>();
	entityWorld.RegisterSystem<transform::TransformSystem>();
	entityWorld.RegisterSystem<visual::VisualSystem>();
}