#include "InventoryPCH.h"
#include "SharedInventory/InventoryRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "SharedInventory/InventoryMemberAddRequestComponent.h"
#include "SharedInventory/InventoryMemberAddResultComponent.h"
#include "SharedInventory/InventoryMemberComponent.h"
#include "SharedInventory/InventoryMemberMoveRequestComponent.h"
#include "SharedInventory/InventoryMemberMoveResultComponent.h"
#include "SharedInventory/InventoryMemberRemoveRequestComponent.h"
#include "SharedInventory/InventoryMemberRemoveResultComponent.h"
#include "SharedInventory/InventoryMemberSystem.h"
#include "SharedInventory/InventoryOwnerComponent.h"
#include "SharedInventory/InventoryOwnerSystem.h"
#include "SharedInventory/InventoryStorageChangesComponent.h"
#include "SharedInventory/InventoryStorageComponent.h"
#include "SharedInventory/InventoryStorageCreateRequestComponent.h"
#include "SharedInventory/InventoryStorageCreateResultComponent.h"
#include "SharedInventory/InventoryStorageDestroyRequestComponent.h"
#include "SharedInventory/InventoryStorageDestroyResultComponent.h"
#include "SharedInventory/InventoryStorageSystem.h"

void inventory::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<inventory::MemberAddRequestComponent>();
	world.RegisterComponent<inventory::MemberAddResultComponent>();
	world.RegisterComponent<inventory::MemberComponent>();
	world.RegisterComponent<inventory::MemberMoveRequestComponent>();
	world.RegisterComponent<inventory::MemberMoveResultComponent>();
	world.RegisterComponent<inventory::MemberRemoveRequestComponent>();
	world.RegisterComponent<inventory::MemberRemoveResultComponent>();
	world.RegisterComponent<inventory::OwnerComponent>();
	world.RegisterComponent<inventory::StorageChangesComponent>();
	world.RegisterComponent<inventory::StorageComponent>();
	world.RegisterComponent<inventory::StorageCreateRequestComponent>();
	world.RegisterComponent<inventory::StorageCreateResultComponent>();
	world.RegisterComponent<inventory::StorageDestroyRequestComponent>();
	world.RegisterComponent<inventory::StorageDestroyResultComponent>();
	world.RegisterSystem<inventory::MemberSystem>();
	world.RegisterSystem<inventory::OwnerSystem>();
	world.RegisterSystem<inventory::StorageSystem>();
}