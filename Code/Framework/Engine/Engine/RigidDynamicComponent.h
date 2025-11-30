#pragma once

#include "ECS/Component.h"

namespace physx
{
	class PxRigidDynamic;
	class PxShape;
}

namespace eng
{
	struct RigidDynamicComponent final : public ecs::Component<RigidDynamicComponent>
	{
		physx::PxRigidDynamic* m_Actor = nullptr;
		Array<physx::PxShape*> m_Shapes = { };
		bool m_IsKinematic = false;
	};
}