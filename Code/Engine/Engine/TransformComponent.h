#pragma once

#include "ECS/Component.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"

class Matrix4x4;

namespace eng
{
	struct TransformComponent : public ecs::Component<TransformComponent>
	{
		Matrix4x4 ToTransform() const;

		Vector3f m_Translate = Vector3f::Zero;
		Rotator m_Rotate = Rotator::Zero;
		Vector3f m_Scale = Vector3f::One;
	};
}