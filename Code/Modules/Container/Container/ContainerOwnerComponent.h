#pragma once

#include "Core/SparseArray.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"

namespace container
{
	/// \brief 
	struct OwnerComponent final : public ecs::Component<OwnerComponent>
	{
		// All the storages the entity owns.
		SparseArray<int32, ecs::Entity> m_Storages = { };
	};
}