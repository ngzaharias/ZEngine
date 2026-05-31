#pragma once

#include "Core/Guid.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "SharedInventory/InventoryErrorEnum.h"

namespace inventory
{
	/// \brief Result of a request made to a member component.
	struct MemberAddResultComponent final : public ecs::FrameComponent
	{
		str::Guid m_TransactionId = { };
		ecs::Entity m_Member = { };
		EError m_Error = { };
	};
}