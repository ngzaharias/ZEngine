#pragma once

#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "ServerInventory/InventoryMemberEnum.h"

namespace server::inventory
{
	/// \brief Holds all information of a single member inside a container.
	struct MemberComponent final : public ecs::Component
	{
		ecs::Entity m_Storage = { };
		uint16 m_Count = 0;
		uint16 m_GridX = 0;
		uint16 m_GridY = 0;
		uint16 m_Type = static_cast<uint16>(EMember::None);
	};
}