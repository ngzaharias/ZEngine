#include "DebugCraftingPCH.h"
#include "DebugCrafting/CraftingRegisterModule.h"

#include "DebugCrafting/CraftingWindowComponent.h"
#include "DebugCrafting/CraftingWindowEvent.h"
#include "DebugCrafting/CraftingWindowSystem.h"
#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"

void debug::crafting::RegisterModule(ecs::EntityWorld& clientWorld, ecs::EntityWorld& serverWorld)
{
	clientWorld.RegisterComponent<debug::crafting::WindowComponent>();
	clientWorld.RegisterEvent<debug::crafting::WindowEvent>();
	clientWorld.RegisterSystem<debug::crafting::WindowSystem>();
}