#include "GameClientPCH.h"
#include "GameClient/RegisterComponents.h"

#include <ECS/EntityWorld.h>

#include "GameClient/ContainerComponents.h"
#include "GameClient/DragComponents.h"
#include "GameClient/GameStateComponents.h"
#include "GameClient/ModalComponents.h"

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
	entityWorld.RegisterComponent<container::StorageChangesComponent>();
	entityWorld.RegisterComponent<container::StorageComponent>();
	entityWorld.RegisterComponent<container::StorageCreateRequestComponent>();
	entityWorld.RegisterComponent<container::StorageCreateResultComponent>();
	entityWorld.RegisterComponent<container::StorageDestroyRequestComponent>();
	entityWorld.RegisterComponent<container::StorageDestroyResultComponent>();
	entityWorld.RegisterComponent<drag::SelectionComponent>();
	entityWorld.RegisterComponent<gamestate::NetworkHostComponent>();
	entityWorld.RegisterComponent<gamestate::NetworkJoinComponent>();
	entityWorld.RegisterComponent<gamestate::NetworkStopComponent>();
	entityWorld.RegisterComponent<gamestate::RequestComponent>();
	entityWorld.RegisterComponent<gamestate::StateComponent>();
	entityWorld.RegisterComponent<gamestate::StateFinishedComponent>();
	entityWorld.RegisterComponent<gui::modal::MessageComponent>();
}
