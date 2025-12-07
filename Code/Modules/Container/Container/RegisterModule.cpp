#include "ContainerPCH.h"
#include "Container/RegisterModule.h"

#include "Container/ContainerComponents.h"
#include "Container/ContainerMemberSystem.h"
#include "Container/ContainerOwnerSystem.h"
#include "Container/ContainerStorageSystem.h"
#include "ECS/EntityWorld.h"
#include "ECS/WorldView.h"
#include "Engine/PrototypeManager.h"

void container::RegisterModule(ecs::EntityWorld& entityWorld)
{
	// components
	{
		entityWorld.RegisterComponent<container::MemberAddRequestComponent>();
		entityWorld.RegisterComponent<container::MemberAddResultComponent>();
		entityWorld.RegisterComponent<container::MemberComponent>();
		entityWorld.RegisterComponent<container::MemberMoveRequestComponent>();
		entityWorld.RegisterComponent<container::MemberMoveResultComponent>();
		entityWorld.RegisterComponent<container::MemberRemoveRequestComponent>();
		entityWorld.RegisterComponent<container::MemberRemoveResultComponent>();
		entityWorld.RegisterComponent<container::OwnerComponent>();
		entityWorld.RegisterComponent<container::StorageComponent>();
		entityWorld.RegisterComponent<container::StorageCreateRequestComponent>();
		entityWorld.RegisterComponent<container::StorageCreateResultComponent>();
		entityWorld.RegisterComponent<container::StorageDestroyRequestComponent>();
		entityWorld.RegisterComponent<container::StorageDestroyResultComponent>();
	}

	// singletons
	{
		entityWorld.RegisterSingleton<container::StorageChangesSingleton>();
	}

	// systems
	{
		entityWorld.RegisterSystem<container::StorageSystem>();
		// #todo: container::MemberSystem depends on container::StorageSystem
		entityWorld.RegisterSystem<container::MemberSystem>();
		// #todo: container::OwnerSystem depends on container::StorageSystem
		entityWorld.RegisterSystem<container::OwnerSystem>();
	}

	// prototypes
	{
	}
}