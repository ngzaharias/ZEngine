#include "DebugInventoryPCH.h"
#include "DebugInventory/InventoryRegisterModule.h"

#include "DebugInventory/InventoryWindowComponent.h"
#include "DebugInventory/InventoryWindowEvent.h"
#include "DebugInventory/InventoryWindowSystem.h"
#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"

void debug::inventory::RegisterModule(ecs::EntityWorld& clientWorld, ecs::EntityWorld& serverWorld)
{
	clientWorld.RegisterComponent<debug::inventory::WindowComponent>();
	clientWorld.RegisterEvent<debug::inventory::WindowEvent>();
	clientWorld.RegisterSystem<debug::inventory::WindowSystem>();
}