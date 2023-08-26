#include "GameClientPCH.h"
#include "GameClient/RegisterComponents.h"

#include <ECS/EntityWorld.h>

#include "GameClient/DragComponents.h"
#include "GameClient/GameStateComponents.h"
#include "GameClient/ModalComponents.h"

void clt::RegisterComponents(ecs::EntityWorld& entityWorld)
{
	entityWorld.RegisterComponent<drag::SelectionComponent>();
	entityWorld.RegisterComponent<gamestate::NetworkHostComponent>();
	entityWorld.RegisterComponent<gamestate::NetworkJoinComponent>();
	entityWorld.RegisterComponent<gamestate::NetworkStopComponent>();
	entityWorld.RegisterComponent<gamestate::RequestComponent>();
	entityWorld.RegisterComponent<gamestate::StateComponent>();
	entityWorld.RegisterComponent<gamestate::StateFinishedComponent>();
	entityWorld.RegisterComponent<gui::modal::MessageComponent>();
}
