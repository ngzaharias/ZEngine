#pragma once

#include "ECS/Component.h"

namespace physx
{
	class PxScene;
}

namespace eng
{
	struct PhysicsSceneComponent : ecs::SingletonComponent<PhysicsSceneComponent>
	{
		physx::PxScene* m_PhysicsScene = nullptr;
	};
}