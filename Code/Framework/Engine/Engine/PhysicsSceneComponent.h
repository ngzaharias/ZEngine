#pragma once

#include "ECS/Component.h"

namespace physx
{
	class PxScene;
}

namespace eng
{
	struct PhysicsSceneSingleton final : public ecs::Singleton<PhysicsSceneSingleton>
	{
		physx::PxScene* m_PhysicsScene = nullptr;
	};
}