#pragma once

#include "Core/Variant.h"
#include "ECS/Component.h"
#include "Engine/CameraTypes.h"
#include "Math/Math.h"
#include "Math/Vector.h"

namespace eng
{
	struct CameraComponent : public ecs::Component<CameraComponent>
	{
		::camera::Projection m_Projection = ::camera::Perspective();
		::camera::EBehaviour m_Behaviour = ::camera::EBehaviour::Free3D;
		float m_ZoomMax = KINDA_LARGE_FLOAT;
		float m_ZoomMin = 1.f;
	};
}