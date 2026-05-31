#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "SharedInventory/InventoryErrorEnum.h"

namespace inventory
{
	/// \brief Result of a destroy request made to a storage entity.
	struct StorageDestroyResultComponent final : public ecs::FrameComponent
	{
		str::Guid m_TransactionId = { };
		ecs::Entity m_Storage = { };
		EError m_Error = { };
	};
}