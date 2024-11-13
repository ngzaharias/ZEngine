#include "GameClientPCH.h"
#include "GameClient/RegisterComponents.h"

#include "ECS/EntityWorld.h"
#include "GameClient/DragComponents.h"
#include "GameClient/GameStateComponents.h"
#include "GameClient/HexmapChartComponent.h"
#include "GameClient/HexmapFragmentComponent.h"
#include "GameClient/HexmapRequestComponent.h"
#include "GameClient/HiddenCountComponent.h"
#include "GameClient/HiddenGroupComponent.h"
#include "GameClient/HiddenObjectComponent.h"
#include "GameClient/HiddenRevealComponent.h"
#include "GameClient/ModalComponents.h"
#include "GameClient/MovementComponents.h"
#include "GameClient/ProjectileComponents.h"
#include "GameClient/SettingsComponents.h"

void clt::RegisterComponents(ecs::EntityWorld& entityWorld)
{
	entityWorld.RegisterComponent<drag::IsSelectableComponent>();
	entityWorld.RegisterComponent<drag::SelectionComponent>();
	entityWorld.RegisterComponent<gamestate::RequestComponent>();
	entityWorld.RegisterComponent<gamestate::StateFinishedComponent>();
	entityWorld.RegisterComponent<gui::modal::MessageComponent>();
	entityWorld.RegisterComponent<hexmap::FragmentComponent>();
	entityWorld.RegisterComponent<hexmap::RequestComponent>();
	entityWorld.RegisterComponent<hidden::CountComponent>();
	entityWorld.RegisterComponent<hidden::GroupComponent>();
	entityWorld.RegisterComponent<hidden::ObjectComponent>();
	entityWorld.RegisterComponent<hidden::RevealComponent>();
	entityWorld.RegisterComponent<movement::AccelerationComponent>();
	entityWorld.RegisterComponent<movement::VelocityComponent>();
	entityWorld.RegisterComponent<projectile::CreateRequestComponent>();
	entityWorld.RegisterComponent<projectile::CreateResultComponent>();
	entityWorld.RegisterComponent<projectile::SpawnComponent>();
	entityWorld.RegisterComponent<projectile::TrajectoryComponent>();

	entityWorld.RegisterSingleton<clt::settings::DebugComponent>();
	entityWorld.RegisterSingleton<gamestate::NetworkJoinComponent>();
	entityWorld.RegisterSingleton<gamestate::StateComponent>();
	entityWorld.RegisterSingleton<hexmap::ChartComponent>();
	entityWorld.RegisterSingleton<projectile::ChangesComponent>();
}
