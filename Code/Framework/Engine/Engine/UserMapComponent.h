#pragma once

#include "Core/Map.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "Network/UserId.h"

namespace net
{
	/// \brief StaticComponent that allows mapping between UserId and Entity.
	struct UserMapComponent final : public ecs::StaticComponent
	{
		Map<ecs::Entity, net::UserId> m_EntityToUser;
		Map<net::UserId, ecs::Entity> m_UserToEntity;
	};
}