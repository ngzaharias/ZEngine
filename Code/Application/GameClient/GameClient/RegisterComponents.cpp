#include "GameClientPCH.h"
#include "GameClient/RegisterComponents.h"

#include "ECS/EntityWorld.h"
#include "GameClient/DragIsSelectableComponent.h"
#include "GameClient/DragSelectionComponent.h"
#include "GameClient/GameStateChangeFinishedEvent.h"
#include "GameClient/GameStateChangeRequestEvent.h"
#include "GameClient/GameStateStateSingleton.h"
#include "GameClient/ModalMessageComponent.h"
#include "GameClient/MovementAccelerationComponent.h"
#include "GameClient/MovementVelocityComponent.h"
#include "GameClient/ProjectileChangesSingleton.h"
#include "GameClient/ProjectileCreateRequestComponent.h"
#include "GameClient/ProjectileCreateResultComponent.h"
#include "GameClient/ProjectileSpawnComponent.h"
#include "GameClient/ProjectileTrajectoryComponent.h"
#include "GameClient/SettingsDebugSingleton.h"

void client::RegisterComponents(ecs::EntityWorld& entityWorld)
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

	entityWorld.RegisterEvent<gamestate::ChangeFinishedEvent>();
	entityWorld.RegisterEvent<gamestate::ChangeRequestEvent>();

	entityWorld.RegisterSingleton<client::settings::DebugSingleton>();
	entityWorld.RegisterSingleton<gamestate::StateSingleton>();
	entityWorld.RegisterSingleton<projectile::ChangesSingleton>();
}
