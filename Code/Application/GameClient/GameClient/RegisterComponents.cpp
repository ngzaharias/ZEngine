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
#include "Hexmap/HexmapLayerComponent.h"
#include "Hexmap/HexmapRootComponent.h"
#include "Hidden/HiddenCountComponent.h"
#include "Hidden/HiddenGroupComponent.h"
#include "Hidden/HiddenObjectComponent.h"
#include "Hidden/HiddenRevealComponent.h"
#include "Hidden/HiddenSettingsComponents.h"
#include "Hidden/HiddenVFXComponent.h"
#include "Softbody/SoftbodyComponents.h"
#include "Tabletop/TabletopCardComponent.h"
#include "Tabletop/TabletopDeckComponent.h"

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
	entityWorld.RegisterComponent<gamestate::RequestComponent>();
	entityWorld.RegisterComponent<gamestate::StateFinishedComponent>();
	entityWorld.RegisterComponent<gui::modal::MessageComponent>();
	entityWorld.RegisterComponent<hexmap::LayerComponent>();
	entityWorld.RegisterComponent<hexmap::RootComponent>();
	entityWorld.RegisterComponent<hidden::CountComponent>();
	entityWorld.RegisterComponent<hidden::GroupComponent>();
	entityWorld.RegisterComponent<hidden::ObjectComponent>();
	entityWorld.RegisterComponent<hidden::RevealComponent>();
	entityWorld.RegisterComponent<hidden::VFXComponent>();
	entityWorld.RegisterComponent<movement::AccelerationComponent>();
	entityWorld.RegisterComponent<movement::VelocityComponent>();
	entityWorld.RegisterComponent<projectile::CreateRequestComponent>();
	entityWorld.RegisterComponent<projectile::CreateResultComponent>();
	entityWorld.RegisterComponent<projectile::SpawnComponent>();
	entityWorld.RegisterComponent<projectile::TrajectoryComponent>();
	entityWorld.RegisterComponent<softbody::ChainComponent>();
	entityWorld.RegisterComponent<tabletop::CardComponent>();
	entityWorld.RegisterComponent<tabletop::DeckComponent>();

	entityWorld.RegisterSingleton<container::StorageChangesComponent>();
	entityWorld.RegisterSingleton<clt::settings::DebugComponent>();
	entityWorld.RegisterSingleton<gamestate::NetworkJoinComponent>();
	entityWorld.RegisterSingleton<gamestate::StateComponent>();
	entityWorld.RegisterSingleton<hidden::settings::DebugComponent>();
	entityWorld.RegisterSingleton<hidden::settings::LocalComponent>();
	entityWorld.RegisterSingleton<projectile::ChangesComponent>();
}
