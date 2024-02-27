#pragma once

#include <Core/Rotator.h>
#include <Core/Vector.h>

#include <ECS/Component.h>

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