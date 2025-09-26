#include "GameClientPCH.h"
#include "GameClient/RegisterSystems.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/RenderSystem.h"
#include "Engine/ReplicationHost.h"
#include "GameClient/AccelerationSystem.h"
#include "GameClient/AchievementSystem.h"
#include "GameClient/ContainerMemberSystem.h"
#include "GameClient/ContainerOwnerSystem.h"
#include "GameClient/ContainerStorageSystem.h"
#include "GameClient/DragMovementSystem.h"
#include "GameClient/DragSelectionSystem.h"
#include "GameClient/GameStateSystem.h"
#include "GameClient/ModalStateSystem.h"
#include "GameClient/NetworkHostSystem.h"
#include "GameClient/NetworkJoinSystem.h"
#include "GameClient/NetworkStopSystem.h"
#include "GameClient/ProjectileSpawnSystem.h"
#include "GameClient/ProjectileTrajectorySystem.h"
#include "GameClient/RenderStage_Hexmap.h"
#include "GameClient/SettingsSystem.h"
#include "GameClient/TransformSystem.h"
#include "GameClient/VelocitySystem.h"
#include "GameClient/VisualSystem.h"

void clt::RegisterSystems(ecs::EntityWorld& entityWorld)
{
	entityWorld.RegisterSystem<client::AchievementSystem>();
	entityWorld.RegisterSystem<client::SettingsSystem>();
	entityWorld.RegisterSystem<container::StorageSystem>();
	// #todo: container::MemberSystem depends on container::StorageSystem
	entityWorld.RegisterSystem<container::MemberSystem>();
	// #todo: container::OwnerSystem depends on container::StorageSystem
	entityWorld.RegisterSystem<container::OwnerSystem>();
	entityWorld.RegisterSystem<drag::MovementSystem>();
	entityWorld.RegisterSystem<drag::SelectionSystem>();
	entityWorld.RegisterSystem<gamestate::NetworkHostSystem>();
	entityWorld.RegisterSystem<gamestate::NetworkJoinSystem>();
	entityWorld.RegisterSystem<gamestate::NetworkStopSystem>();
	entityWorld.RegisterSystem<gamestate::StateSystem>();
	entityWorld.RegisterSystem<gui::modal::StateSystem>();
	entityWorld.RegisterSystem<movement::AccelerationSystem>();
	entityWorld.RegisterSystem<movement::VelocitySystem>();
	entityWorld.RegisterSystem<projectile::SpawnSystem>();
	entityWorld.RegisterSystem<projectile::TrajectorySystem>();
	entityWorld.RegisterSystem<transform::TransformSystem>();
	entityWorld.RegisterSystem<visual::VisualSystem>();

	// SettingsSystem system needs to run before WindowSystem
	entityWorld.RegisterSystemPriority<client::SettingsSystem>(1);
}