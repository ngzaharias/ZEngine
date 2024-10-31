#include "GameClientPCH.h"
#include "GameClient/RegisterSystems.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/ReplicationHost.h"
#include "GameClient/AccelerationSystem.h"
#include "GameClient/AchievementSystem.h"
#include "GameClient/DragMovementSystem.h"
#include "GameClient/DragSelectionSystem.h"
#include "GameClient/GameStateSystem.h"
#include "GameClient/HexamapGridSystem.h"
#include "GameClient/HiddenCountSystem.h"
#include "GameClient/HiddenPhysicsSystem.h"
#include "GameClient/HiddenRevealSystem.h"
#include "GameClient/HiddenSoundSystem.h"
#include "GameClient/HiddenSpriteSystem.h"
#include "GameClient/HiddenTrackerSystem.h"
#include "GameClient/HiddenVFXSystem.h"
#include "GameClient/ModalStateSystem.h"
#include "GameClient/NetworkHostSystem.h"
#include "GameClient/NetworkJoinSystem.h"
#include "GameClient/NetworkStopSystem.h"
#include "GameClient/ProjectileSpawnSystem.h"
#include "GameClient/ProjectileTrajectorySystem.h"
#include "GameClient/TransformSystem.h"
#include "GameClient/VelocitySystem.h"
#include "GameClient/VisualSystem.h"

void clt::RegisterSystems(ecs::EntityWorld& entityWorld)
{
	entityWorld.RegisterSystem<clt::AchievementSystem>();
	entityWorld.RegisterSystem<drag::MovementSystem>();
	entityWorld.RegisterSystem<drag::SelectionSystem>();
	entityWorld.RegisterSystem<gamestate::NetworkHostSystem>();
	entityWorld.RegisterSystem<gamestate::NetworkJoinSystem>();
	entityWorld.RegisterSystem<gamestate::NetworkStopSystem>();
	entityWorld.RegisterSystem<gamestate::StateSystem>();
	entityWorld.RegisterSystem<gui::modal::StateSystem>();
	entityWorld.RegisterSystem<hexamap::GridSystem>();
	entityWorld.RegisterSystem<hidden::CountSystem>();
	entityWorld.RegisterSystem<hidden::PhysicsSystem>();
	entityWorld.RegisterSystem<hidden::RevealSystem>();
	entityWorld.RegisterSystem<hidden::SoundSystem>();
	entityWorld.RegisterSystem<hidden::SpriteSystem>();
	entityWorld.RegisterSystem<hidden::TrackerSystem>();
	entityWorld.RegisterSystem<hidden::VFXSystem>();
	entityWorld.RegisterSystem<movement::AccelerationSystem>();
	entityWorld.RegisterSystem<movement::VelocitySystem>();
	entityWorld.RegisterSystem<projectile::SpawnSystem>();
	entityWorld.RegisterSystem<projectile::TrajectorySystem>();
	entityWorld.RegisterSystem<transform::TransformSystem>();
	entityWorld.RegisterSystem<visual::VisualSystem>();
}