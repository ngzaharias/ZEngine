#pragma once

#include "Core/Variant.h"
#include "ECS/Component.h"
#include "Engine/CameraTypes.h"
#include "Math/Math.h"
#include "Math/Vector.h"

namespace eng::camera
{
	struct BehaviourComponent : public ecs::Component<BehaviourComponent>
	{
		eng::camera::EBehaviour m_Behaviour = eng::camera::EBehaviour::Free3D;
		Vector3f m_FrustrumEdgeMax = Vector3f(+KINDA_LARGE_FLOAT);
		Vector3f m_FrustrumEdgeMin = Vector3f(-KINDA_LARGE_FLOAT);
		float m_ZoomMax = KINDA_LARGE_FLOAT;
		float m_ZoomMin = 1.f;
	};

	struct ProjectionComponent : public ecs::Component<ProjectionComponent>
	{
		eng::camera::Projection m_Projection = eng::camera::Perspective();
	};
}