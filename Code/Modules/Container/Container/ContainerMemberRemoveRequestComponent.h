#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"

namespace container
{
	/// \brief Make a request for a member component.
	struct MemberRemoveRequestComponent final : public ecs::Component<MemberRemoveRequestComponent>
	{
		str::Guid m_TransactionId = { };
		ecs::Entity m_Member = { };
	};
}