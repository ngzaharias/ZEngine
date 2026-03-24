#include "ServerInventoryPCH.h"
#include "ServerInventory/InventoryRegisterModule.h"

#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/PrototypeManager.h"
#include "ServerInventory/InventoryMemberAddRequestComponent.h"
#include "ServerInventory/InventoryMemberAddResultComponent.h"
#include "ServerInventory/InventoryMemberComponent.h"
#include "ServerInventory/InventoryMemberMoveRequestComponent.h"
#include "ServerInventory/InventoryMemberMoveResultComponent.h"
#include "ServerInventory/InventoryMemberRemoveRequestComponent.h"
#include "ServerInventory/InventoryMemberRemoveResultComponent.h"
#include "ServerInventory/InventoryMemberSystem.h"
#include "ServerInventory/InventoryOwnerComponent.h"
#include "ServerInventory/InventoryOwnerSystem.h"
#include "ServerInventory/InventoryStorageChangesStaticComponent.h"
#include "ServerInventory/InventoryStorageComponent.h"
#include "ServerInventory/InventoryStorageCreateRequestComponent.h"
#include "ServerInventory/InventoryStorageCreateResultComponent.h"
#include "ServerInventory/InventoryStorageDestroyRequestComponent.h"
#include "ServerInventory/InventoryStorageDestroyResultComponent.h"
#include "ServerInventory/InventoryStorageSystem.h"

void server::inventory::RegisterModule(ecs::EntityWorld& world)
{
	world.RegisterComponent<server::inventory::MemberAddRequestComponent>();
	world.RegisterComponent<server::inventory::MemberAddResultComponent>();
	world.RegisterComponent<server::inventory::MemberComponent>();
	world.RegisterComponent<server::inventory::MemberMoveRequestComponent>();
	world.RegisterComponent<server::inventory::MemberMoveResultComponent>();
	world.RegisterComponent<server::inventory::MemberRemoveRequestComponent>();
	world.RegisterComponent<server::inventory::MemberRemoveResultComponent>();
	world.RegisterComponent<server::inventory::OwnerComponent>();
	world.RegisterComponent<server::inventory::StorageComponent>();
	world.RegisterComponent<server::inventory::StorageCreateRequestComponent>();
	world.RegisterComponent<server::inventory::StorageCreateResultComponent>();
	world.RegisterComponent<server::inventory::StorageDestroyRequestComponent>();
	world.RegisterComponent<server::inventory::StorageDestroyResultComponent>();
	world.RegisterComponent<server::inventory::StorageChangesStaticComponent>();
	world.RegisterSystem<server::inventory::StorageSystem>();
	world.RegisterSystem<server::inventory::MemberSystem>();
	world.RegisterSystem<server::inventory::OwnerSystem>();
}