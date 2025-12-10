#pragma once

#include "Core/Array.h"
#include "ECS/Component.h"
#include "ECS/Entity.h"

namespace projectile
{
	struct Created
	{
		ecs::Entity m_Projectile = { };
		ecs::Entity m_Request = { };
	};

	struct Destroyed
	{
		ecs::Entity m_Projectile = { };
	};

	/// \brief 
	struct ChangesSingleton final : public ecs::Singleton<ChangesSingleton>
	{
		Array<Created> m_Created = { };
		Array<Destroyed> m_Destroyed = { };
	};
}