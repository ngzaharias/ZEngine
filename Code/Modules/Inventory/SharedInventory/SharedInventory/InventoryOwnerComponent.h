#pragma once

#include "Core/SparseSet.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"

namespace shared::inventory
{
	/// \brief 
	struct OwnerComponent final : public ecs::Component
	{
		// All the storages the entity owns.
		SparseSet<int32, ecs::Entity> m_Storages = { };
	};
}