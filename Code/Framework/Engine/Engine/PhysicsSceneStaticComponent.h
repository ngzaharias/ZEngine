#pragma once

#include "ECS/Component.h"

namespace physx
{
	class PxScene;
}

namespace eng
{
	struct PhysicsSceneStaticComponent final : public ecs::StaticComponent
	{
		physx::PxScene* m_PhysicsScene = nullptr;
	};
}