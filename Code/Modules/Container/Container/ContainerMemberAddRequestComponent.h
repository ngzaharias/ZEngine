#pragma once

#include "Container/ContainerMemberEnum.h"
#include "Core/Guid.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"

namespace container
{
	/// \brief Make a request for a member component.
	struct MemberAddRequestComponent final : public ecs::Component<MemberAddRequestComponent>
	{
		str::Guid m_TransactionId = { };
		// Storage that it is being added to.
		ecs::Entity m_Storage = { };

		// Member that is being added.
		ecs::Entity m_Member = { };
		uint16 m_Count = 1;
		uint16 m_GridX = 0;
		uint16 m_GridY = 0;
		uint16 m_Type = static_cast<uint16>(EMember::None);
	};
}