#pragma once

#include <Core/Rotator.h>
#include <Core/Vector.h>

#include <ECS/Component.h>

namespace eng
{
	struct TransformComponent : public ecs::Component<TransformComponent>
	{
		Vector3f m_Translate = Vector3f::Zero;
		Rotator m_Rotate = Rotator::Zero;
		Vector3f m_Scale = Vector3f::One;
	};
}