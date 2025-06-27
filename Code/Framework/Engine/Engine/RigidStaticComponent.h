#pragma once

#include "ECS/Component.h"

namespace physx
{
	class PxRigidStatic;
	class PxShape;
}

namespace eng
{
	struct RigidStaticComponent : public ecs::Component<RigidStaticComponent>
	{
		physx::PxRigidStatic* m_Actor = nullptr;
		Array<physx::PxShape*> m_Shapes = { };
	};
}