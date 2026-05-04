#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"

namespace inventory
{
	/// \brief Make a request for a member component.
	struct MemberMoveRequestComponent final : public ecs::Component
	{
		str::Guid m_TransactionId = { };
		ecs::Entity m_Storage = { };
		ecs::Entity m_Member = { };
	};
}