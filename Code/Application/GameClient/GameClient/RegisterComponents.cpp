#include "GameClientPCH.h"
#include "GameClient/RegisterComponents.h"

#include "ECS/EntityWorld.h"
#include "GameClient/DragIsSelectableComponent.h"
#include "GameClient/DragSelectionComponent.h"
#include "GameClient/GameStateChangeFinished.h"
#include "GameClient/GameStateChangeRequest.h"
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

	entityWorld.RegisterEvent<gamestate::ChangeFinished>();
	entityWorld.RegisterEvent<gamestate::ChangeRequest>();

	entityWorld.RegisterSingleton<client::settings::DebugSingleton>();
	entityWorld.RegisterSingleton<gamestate::StateSingleton>();
	entityWorld.RegisterSingleton<projectile::ChangesSingleton>();
}
