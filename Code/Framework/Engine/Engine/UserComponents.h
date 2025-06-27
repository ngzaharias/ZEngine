#pragma once

#include "Core/Map.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "Network/UserId.h"

namespace net
{
	/// \brief Attached to the entity that is created when a user first connects to the server.
	struct UserComponent final : ecs::Component<UserComponent>
	{
		net::UserId m_UserId = net::UserId::Unassigned;
	};

	/// \brief Singleton that allows mapping between UserId and Entity.
	struct UserMapComponent final : ecs::SingletonComponent<UserMapComponent>
	{
		Map<ecs::Entity, net::UserId> m_EntityToUser;
		Map<net::UserId, ecs::Entity> m_UserToEntity;
	};
}