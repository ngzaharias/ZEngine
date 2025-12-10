#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "Math/Vector.h"

namespace projectile
{
	enum class EError
	{
		None,
		// projectile entity is dead
		ProjectileDead,
	};

	/// \brief Result of a spawn request made to a storage entity.
	struct CreateResultComponent final : public ecs::Component<CreateResultComponent>
	{
		// The unique id to track the projectile.
		str::Guid m_TransactionId = { };
		// The projectile that was created, unassigned if it failed.
		ecs::Entity m_Projectile = { };
		// Error result when creating the projectile.
		EError m_Error = { };
	};
}