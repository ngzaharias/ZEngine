#include "TacticsPCH.h"
#include "ClientTactics/TacticsRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/TableHeadmaster.h"
#include "Engine/TemplateManager.h"
#include "ClientTactics/TacticsAbilityPreviewComponent.h"
#include "ClientTactics/TacticsAbilityPreviewEvent.h"
#include "ClientTactics/TacticsAbilityPreviewSystem.h"
#include "ClientTactics/TacticsPawnSelectedComponent.h"
#include "ClientTactics/TacticsPawnSelectionSystem.h"

void client::tactics::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<client::tactics::AbilityPreviewComponent>();
	world.RegisterComponent<client::tactics::PawnSelectedComponent>();
	world.RegisterEvent<client::tactics::AbilityPreviewEvent>();
	world.RegisterSystem<client::tactics::AbilityPreviewSystem>();
	world.RegisterSystem<client::tactics::PawnSelectionSystem>();
}