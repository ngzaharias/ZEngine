#pragma once

#include "ECS/Component.h"
#include "ECS/IsPrototype.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"

class Matrix4x4;

namespace eng
{
	struct TransformComponent final : public ecs::Component<TransformComponent>
		, ecs::IsPrototype
	{
		Matrix4x4 ToTransform() const;

		Vector3f m_Translate = Vector3f::Zero;
		Rotator m_Rotate = Rotator::Zero;
		Vector3f m_Scale = Vector3f::One;
	};
}