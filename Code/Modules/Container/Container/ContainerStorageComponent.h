#pragma once

#include "Container/ContainerStorageEnum.h"
#include "Core/Set.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"

namespace container
{
	/// \brief Holds a list of entities for a container.
	struct StorageComponent final : public ecs::Component<StorageComponent>
	{
		ecs::Entity m_Owner = { };
		int32 m_Limit = INT32_MAX;
		int32 m_Type = static_cast<int32>(EStorage::Main);

		// All the members of the storage.
		Set<ecs::Entity> m_Members = { };
	};
}