#include "GameClientPCH.h"
#include "GameClient/RegisterComponents.h"

#include "ECS/EntityWorld.h"
#include "GameClient/DragComponents.h"
#include "GameClient/GameStateChangeFinished.h"
#include "GameClient/GameStateChangeRequest.h"
#include "GameClient/GameStateNetworkJoinSingleton.h"
#include "GameClient/GameStateStateSingleton.h"
#include "GameClient/ModalMessageComponent.h"
#include "GameClient/MovementAccelerationComponent.h"
#include "GameClient/MovementVelocityComponent.h"
#include "GameClient/ProjectileChangesSingleton.h"
#include "GameClient/ProjectileCreateRequestComponent.h"
#include "GameClient/ProjectileCreateResultComponent.h"
#include "GameClient/ProjectileSpawnComponent.h"
#include "GameClient/ProjectileTrajectoryComponent.h"
#include "GameClient/SettingsDebugComponent.h"

void clt::RegisterComponents(ecs::EntityWorld& entityWorld)
{
	entityWorld.RegisterComponent<drag::IsSelectableComponent>();
	entityWorld.RegisterComponent<drag::SelectionComponent>();
	entityWorld.RegisterComponent<gui::modal::MessageComponent>();
	entityWorld.RegisterComponent<movement::AccelerationComponent>();
	entityWorld.RegisterComponent<movement::VelocityComponent>();
	entityWorld.RegisterComponent<projectile::CreateRequestComponent>();
	entityWorld.RegisterComponent<projectile::CreateResultComponent>();
	entityWorld.RegisterComponent<projectile::SpawnComponent>();
	entityWorld.RegisterComponent<projectile::TrajectoryComponent>();

	entityWorld.RegisterEvent<gamestate::ChangeFinished>();
	entityWorld.RegisterEvent<gamestate::ChangeRequest>();

	entityWorld.RegisterSingleton<clt::settings::DebugSingleton>();
	entityWorld.RegisterSingleton<gamestate::NetworkJoinSingleton>();
	entityWorld.RegisterSingleton<gamestate::StateSingleton>();
	entityWorld.RegisterSingleton<projectile::ChangesSingleton>();
}
