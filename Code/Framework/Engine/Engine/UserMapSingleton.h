#pragma once

#include "Core/Map.h"
#include "ECS/Singleton.h"
#include "ECS/Entity.h"
#include "Network/UserId.h"

namespace net
{
	/// \brief Singleton that allows mapping between UserId and Entity.
	struct UserMapSingleton final : public ecs::Singleton<UserMapSingleton>
	{
		Map<ecs::Entity, net::UserId> m_EntityToUser;
		Map<net::UserId, ecs::Entity> m_UserToEntity;
	};
}