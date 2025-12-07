#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"

namespace container
{
	/// \brief Request that a new storage is created.
	struct StorageCreateRequestComponent final : public ecs::Component<StorageCreateRequestComponent>
	{
		str::Guid m_TransactionId = { };
		// The owner of the storage, can be unassigned.
		ecs::Entity m_Owner = { };
		// Maximum number of items allowed in the storage.
		int32 m_Limit = INT32_MAX;
		// The storage type.
		int32 m_Type = static_cast<int32>(EStorage::Main);
	};
}