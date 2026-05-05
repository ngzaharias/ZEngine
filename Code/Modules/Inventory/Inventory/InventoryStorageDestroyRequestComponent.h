#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"

namespace inventory
{
	/// \brief Request that the storage entity is destroyed.
	struct StorageDestroyRequestComponent final : public ecs::Component
	{
		str::Guid m_TransactionId = { };
		ecs::Entity m_Storage = { };
	};
}