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

void shared::inventory::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<shared::inventory::MemberAddRequestComponent>();
	world.RegisterComponent<shared::inventory::MemberAddResultComponent>();
	world.RegisterComponent<shared::inventory::MemberComponent>();
	world.RegisterComponent<shared::inventory::MemberMoveRequestComponent>();
	world.RegisterComponent<shared::inventory::MemberMoveResultComponent>();
	world.RegisterComponent<shared::inventory::MemberRemoveRequestComponent>();
	world.RegisterComponent<shared::inventory::MemberRemoveResultComponent>();
	world.RegisterComponent<shared::inventory::OwnerComponent>();
	world.RegisterComponent<shared::inventory::StorageChangesComponent>();
	world.RegisterComponent<shared::inventory::StorageComponent>();
	world.RegisterComponent<shared::inventory::StorageCreateRequestComponent>();
	world.RegisterComponent<shared::inventory::StorageCreateResultComponent>();
	world.RegisterComponent<shared::inventory::StorageDestroyRequestComponent>();
	world.RegisterComponent<shared::inventory::StorageDestroyResultComponent>();
	world.RegisterSystem<shared::inventory::MemberSystem>();
	world.RegisterSystem<shared::inventory::OwnerSystem>();
	world.RegisterSystem<shared::inventory::StorageSystem>();
}