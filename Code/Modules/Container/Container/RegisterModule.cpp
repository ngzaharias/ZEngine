#include "ContainerPCH.h"
#include "Container/RegisterModule.h"

#include "Container/ContainerMemberAddRequestComponent.h"
#include "Container/ContainerMemberAddResultComponent.h"
#include "Container/ContainerMemberComponent.h"
#include "Container/ContainerMemberMoveRequestComponent.h"
#include "Container/ContainerMemberMoveResultComponent.h"
#include "Container/ContainerMemberRemoveRequestComponent.h"
#include "Container/ContainerMemberRemoveResultComponent.h"
#include "Container/ContainerMemberSystem.h"
#include "Container/ContainerOwnerComponent.h"
#include "Container/ContainerOwnerSystem.h"
#include "Container/ContainerStorageChangesSingleton.h"
#include "Container/ContainerStorageComponent.h"
#include "Container/ContainerStorageCreateRequestComponent.h"
#include "Container/ContainerStorageCreateResultComponent.h"
#include "Container/ContainerStorageDestroyRequestComponent.h"
#include "Container/ContainerStorageDestroyResultComponent.h"
#include "Container/ContainerStorageSystem.h"
#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/PrototypeManager.h"

void container::RegisterModule(ecs::EntityWorld& world)
{
	// components
	{
		world.RegisterComponent<container::MemberAddRequestComponent>();
		world.RegisterComponent<container::MemberAddResultComponent>();
		world.RegisterComponent<container::MemberComponent>();
		world.RegisterComponent<container::MemberMoveRequestComponent>();
		world.RegisterComponent<container::MemberMoveResultComponent>();
		world.RegisterComponent<container::MemberRemoveRequestComponent>();
		world.RegisterComponent<container::MemberRemoveResultComponent>();
		world.RegisterComponent<container::OwnerComponent>();
		world.RegisterComponent<container::StorageComponent>();
		world.RegisterComponent<container::StorageCreateRequestComponent>();
		world.RegisterComponent<container::StorageCreateResultComponent>();
		world.RegisterComponent<container::StorageDestroyRequestComponent>();
		world.RegisterComponent<container::StorageDestroyResultComponent>();
	}

	// singletons
	{
		world.RegisterSingleton<container::StorageChangesSingleton>();
	}

	// systems
	{
		world.RegisterSystem<container::StorageSystem>();
		world.RegisterSystem<container::MemberSystem>();
		world.RegisterSystem<container::OwnerSystem>();
	}
}