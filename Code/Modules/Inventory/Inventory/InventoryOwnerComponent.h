#pragma once

#include "Core/SparseArray.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"

namespace inventory
{
	/// \brief 
	struct OwnerComponent final : public ecs::Component
	{
		// All the storages the entity owns.
		SparseArray<int32, ecs::Entity> m_Storages = { };
	};
}