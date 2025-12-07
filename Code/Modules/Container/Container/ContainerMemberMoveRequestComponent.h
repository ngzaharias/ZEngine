#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"

namespace container
{
	/// \brief Make a request for a member component.
	struct MemberMoveRequestComponent final : public ecs::Component<MemberMoveRequestComponent>
	{
		str::Guid m_TransactionId = { };
		ecs::Entity m_Storage = { };
		ecs::Entity m_Member = { };
	};
}