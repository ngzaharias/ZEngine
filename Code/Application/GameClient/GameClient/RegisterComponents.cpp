#include "GameClientPCH.h"
#include "GameClient/RegisterComponents.h"

#include "ECS/EntityWorld.h"
#include "GameClient/ContainerComponents.h"
#include "GameClient/DragComponents.h"
#include "GameClient/GameStateComponents.h"
#include "GameClient/ModalComponents.h"
#include "GameClient/MovementComponents.h"
#include "GameClient/ProjectileComponents.h"
#include "GameClient/SettingsComponents.h"

void clt::RegisterComponents(ecs::EntityWorld& entityWorld)
{
	entityWorld.RegisterComponent<container::MemberAddRequestComponent>();
	entityWorld.RegisterComponent<container::MemberAddResultComponent>();
	entityWorld.RegisterComponent<container::MemberComponent>();
	entityWorld.RegisterComponent<container::MemberMoveRequestComponent>();
	entityWorld.RegisterComponent<container::MemberMoveResultComponent>();
	entityWorld.RegisterComponent<container::MemberRemoveRequestComponent>();
	entityWorld.RegisterComponent<container::MemberRemoveResultComponent>();
	entityWorld.RegisterComponent<container::OwnerComponent>();
	entityWorld.RegisterComponent<container::StorageComponent>();
	entityWorld.RegisterComponent<container::StorageCreateRequestComponent>();
	entityWorld.RegisterComponent<container::StorageCreateResultComponent>();
	entityWorld.RegisterComponent<container::StorageDestroyRequestComponent>();
	entityWorld.RegisterComponent<container::StorageDestroyResultComponent>();
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

	entityWorld.RegisterSingleton<container::StorageChangesSingleton>();
	entityWorld.RegisterSingleton<clt::settings::DebugSingleton>();
	entityWorld.RegisterSingleton<gamestate::NetworkJoinSingleton>();
	entityWorld.RegisterSingleton<gamestate::StateSingleton>();
	entityWorld.RegisterSingleton<projectile::ChangesSingleton>();
}
