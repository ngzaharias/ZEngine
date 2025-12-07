#pragma once

#include "Container/ContainerErrorEnum.h"
#include "Core/Guid.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"

namespace container
{
	/// \brief Result of a create request made to a storage entity.
	struct StorageCreateResultComponent final : public ecs::Component<StorageCreateResultComponent>
	{
		str::Guid m_TransactionId = { };
		// The entity that was created, unassigned if it failed.
		ecs::Entity m_Storage = { };
		// Error result when creating the storage.
		EError m_Error = { };
	};
}