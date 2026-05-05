#include "InventoryPCH.h"
#include "Inventory/InventoryRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Inventory/InventoryMemberAddRequestComponent.h"
#include "Inventory/InventoryMemberAddResultComponent.h"
#include "Inventory/InventoryMemberComponent.h"
#include "Inventory/InventoryMemberMoveRequestComponent.h"
#include "Inventory/InventoryMemberMoveResultComponent.h"
#include "Inventory/InventoryMemberRemoveRequestComponent.h"
#include "Inventory/InventoryMemberRemoveResultComponent.h"
#include "Inventory/InventoryMemberSystem.h"
#include "Inventory/InventoryOwnerComponent.h"
#include "Inventory/InventoryOwnerSystem.h"
#include "Inventory/InventoryStorageChangesComponent.h"
#include "Inventory/InventoryStorageComponent.h"
#include "Inventory/InventoryStorageCreateRequestComponent.h"
#include "Inventory/InventoryStorageCreateResultComponent.h"
#include "Inventory/InventoryStorageDestroyRequestComponent.h"
#include "Inventory/InventoryStorageDestroyResultComponent.h"
#include "Inventory/InventoryStorageSystem.h"

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