#pragma once

#include "ECS/Component.h"
#include "Network/UserId.h"

namespace net
{
	/// \brief Attached to the entity that is created when a user first connects to the server.
	struct UserComponent final : public ecs::Component<UserComponent>
	{
		net::UserId m_UserId = net::UserId::Unassigned;
	};
}