#pragma once

#include "Container/ContainerErrorEnum.h"
#include "Core/Guid.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"

namespace container
{
	/// \brief Result of a request made to a member component.
	struct MemberMoveResultComponent final : public ecs::Component<MemberMoveResultComponent>
	{
		str::Guid m_TransactionId = { };
		ecs::Entity m_Member = { };
		EError m_Error = { };
	};
}