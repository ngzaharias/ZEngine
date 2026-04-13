#pragma once

#include "Core/Array.h"
#include "ECS/Component.h"

namespace physx
{
	class PxRigidActor;
	class PxShape;
}

namespace eng
{
	struct PhysicsComponent final : public ecs::Component
	{
		physx::PxRigidActor* m_PxRigidActor = nullptr;
		Array<physx::PxShape*> m_PxShapes = {};
	};
}